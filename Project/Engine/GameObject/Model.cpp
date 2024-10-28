#include "Model.h"
#include "Engine/Assets/Material.h"
#include "Engine/Assets/PBRMaterial.h"

Model::Model() {
}

Model::~Model() {

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 初期化関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void Model::Init(ID3D12Device* device, const std::string& directorPath, const std::string& fileName) {
	Log("Load: " + fileName + "\n");
	LoadObj(directorPath, fileName, device);

	currentAnimationTime_ = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 更新関数
//////////////////////////////////////////////////////////////////////////////////////////////////
void Model::Update() {
	currentAnimationTime_ += rootNode_.animationsData.tickPerSecond * kDeltaTime_;
	currentAnimationTime_ = fmod(currentAnimationTime_, rootNode_.animationsData.duration);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 描画関数
//////////////////////////////////////////////////////////////////////////////////////////////////
void Model::Draw(ID3D12GraphicsCommandList* commandList,
				 const WorldTransform* worldTransform,
				 const ViewProjection* viewProjection,
				 const std::vector<std::unique_ptr<Material>>& materials) {

	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (uint32_t oi = 0; oi < meshArray_.size(); oi++) {
		meshArray_[oi]->Draw(commandList);
		materials[oi]->Draw(commandList);

		worldTransform->Draw(commandList);
		viewProjection->Draw(commandList);

		if (hasTexture_) {
			std::string textureName = materials[oi]->GetUseTexture();
			TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, textureName, 3);
		}

		commandList->DrawIndexedInstanced(meshArray_[oi]->GetIndexNum(), 1, 0, 0, 0);
	}
}

void Model::DrawSkinning(ID3D12GraphicsCommandList* commandList,
						 const Skinning* skinning,
						 const WorldTransform* worldTransform,
						 const ViewProjection* viewprojection,
						 const std::vector<std::unique_ptr<Material>>& materials) {

	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (uint32_t oi = 0; oi < meshArray_.size(); oi++) {
		skinning->StackCommand(commandList, meshArray_[oi]->GetVBV());
		meshArray_[oi]->DrawIndex(commandList);
		materials[oi]->Draw(commandList);

		worldTransform->Draw(commandList);
		viewprojection->Draw(commandList);

		if (hasTexture_) {
			std::string textureName = materials[oi]->GetUseTexture();
			TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, textureName, 3);
		}

		commandList->DrawIndexedInstanced(meshArray_[oi]->GetIndexNum(), 1, 0, 0, 0);
	}
}

void Model::DrawSkinnings(ID3D12GraphicsCommandList* commandList,
						  const std::vector<std::unique_ptr<Skinning>>& skinning,
						  const WorldTransform* worldTransform,
						  const ViewProjection* viewprojection, 
						  const std::vector<std::unique_ptr<Material>>& materials) {

	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (uint32_t oi = 0; oi < meshArray_.size(); oi++) {
		skinning[oi]->StackCommand(commandList, meshArray_[oi]->GetVBV());
		meshArray_[oi]->DrawIndex(commandList);
		materials[oi]->Draw(commandList);

		worldTransform->Draw(commandList);
		viewprojection->Draw(commandList);

		if (hasTexture_) {
			std::string textureName = materials[oi]->GetUseTexture();
			TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, textureName, 3);
		}

		commandList->DrawIndexedInstanced(meshArray_[oi]->GetIndexNum(), 1, 0, 0, 0);
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void Model::Debug_Gui(const std::string& name) {
	if (ImGui::TreeNode(name.c_str())) {
		for (uint32_t oi = 0; oi < meshArray_.size(); oi++) {
			std::string materialNum = std::to_string(oi);
			std::string materialName = "material" + materialNum;
			if (ImGui::TreeNode(materialName.c_str())) {
				//materialArray_[meshArray_[oi]->GetUseMaterial()]->ImGuiDraw();
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
}
#endif


/// //////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　 assimpでのNode解析
//////////////////////////////////////////////////////////////////////////////////////////////////

Model::Node Model::ReadNode(aiNode* node, const aiScene* scene) {
	Node result;
	// ----------------------------------
	// LocalMatrixを取得する
	// ----------------------------------
	aiVector3D scale, translate;
	aiQuaternion rotate;
	node->mTransformation.Decompose(scale, rotate, translate);

	result.transform.scale = { 1, 1, 1 };
	result.transform.rotate = { rotate.x, -rotate.y, -rotate.z, rotate.w };
	result.transform.translate = { -translate.x, translate.y, translate.z };
	result.localMatrix = MakeAffineMatrix(result.transform.scale, result.transform.rotate.Normalize(), result.transform.translate);
	result.name = node->mName.C_Str(); // Nodeの名前を格納

	// ----------------------------------
	// Nodeを格納する
	// ----------------------------------
	result.children.resize(node->mNumChildren); // 子供の数だけ確保
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
		// 再帰的に読んで階層構造を作っていく
		result.children[childIndex] = ReadNode(node->mChildren[childIndex], scene);
	}

	return result;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Modelの解析
//////////////////////////////////////////////////////////////////////////////////////////////////

void Model::LoadObj(const std::string& directoryPath, const std::string& fileName, ID3D12Device* device) {
	Assimp::Importer importer;
	std::string filePath = directoryPath + fileName;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs |
											 aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);
	if (!scene) {
		std::cerr << "Error: " << importer.GetErrorString() << std::endl;
	}
	assert(scene->HasMeshes()); // meshがないのは対応しない

	std::vector<std::vector<Mesh::VertexData>> meshVertices;
	std::vector<std::vector<uint32_t>> meshIndices;
	std::vector<std::string> useMaterial;

	std::unordered_map<std::string, ModelMaterialData> materialData;
	std::vector<std::string> materials;

	// -------------------------------------------------
	// ↓ meshの解析
	// -------------------------------------------------
	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		std::vector<Mesh::VertexData> triangle;
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals()); // 法線がないなら非対応
		assert(mesh->HasTextureCoords(0)); // texcoordがないmeshは非対応

		meshIndices.resize(scene->mNumMeshes);

		// -------------------------------------------------
		// ↓ faceの解析をする
		// -------------------------------------------------
		std::vector<Mesh::VertexData> vertices;
		vertices.resize(mesh->mNumVertices);
		// vertexの解析を行う
		for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) {
			//uint32_t vertexIndex = .mIndices[element];
			aiVector3D& position = mesh->mVertices[vertexIndex];
			aiVector3D& normal = mesh->mNormals[vertexIndex];
			aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
			aiVector3D& tangent = mesh->mTangents[vertexIndex];

			vertices[vertexIndex].pos = { position.x, position.y, position.z, 1.0f };
			vertices[vertexIndex].normal = { normal.x, normal.y, normal.z };
			vertices[vertexIndex].texcoord = { texcoord.x, texcoord.y };
			vertices[vertexIndex].tangent = { tangent.x, tangent.y, tangent.z };

			vertices[vertexIndex].pos.x *= -1.0f;
			vertices[vertexIndex].normal.x *= -1.0f;
		}

		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);

			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				meshIndices[meshIndex].push_back(vertexIndex);
			}
		}

		// -------------------------------------------------
		// ↓ メッシュのマテリアルインデックスを取得
		// -------------------------------------------------
		uint32_t materialIndex = mesh->mMaterialIndex;
		if (materialIndex < scene->mNumMaterials) {
			aiMaterial* material = scene->mMaterials[materialIndex];
			aiString materialName;
			if (AI_SUCCESS == material->Get(AI_MATKEY_NAME, materialName)) {
				std::string nameStr = materialName.C_Str();
				// DefaultMaterialを除く処理
				if (nameStr == "DefaultMaterial") {
					continue;
				}
				useMaterial.push_back(nameStr);
			}
		}

		// -------------------------------------------------
		// ↓ skinningを取得する用の処理
		// -------------------------------------------------
		std::map<std::string, Skinning::JointWeightData> newMap;
		for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {
			// jointごとの格納領域を作る
			aiBone* bone = mesh->mBones[boneIndex];
			std::string jointName = bone->mName.C_Str();
			Skinning::JointWeightData& jointWeightData = newMap[jointName];

			// InverseBindPoseMatrixの抽出
			aiMatrix4x4 bindPoseMatrixAssimp = bone->mOffsetMatrix.Inverse();
			aiVector3D scale, translate;
			aiQuaternion rotate;
			bindPoseMatrixAssimp.Decompose(scale, rotate, translate);
			Matrix4x4 bindPoseMatrix = MakeAffineMatrix({ scale.x, scale.y, scale.z },
														{ rotate.x, -rotate.y, -rotate.z, rotate.w },
														{ -translate.x, translate.y, translate.z }
			);
			jointWeightData.inverseBindPoseMatrix = bindPoseMatrix.Inverse();

			// Weight情報を取り出す
			for (uint32_t weightIndex = 0; weightIndex < bone->mNumWeights; ++weightIndex) {
				jointWeightData.vertexWeight.push_back({ bone->mWeights[weightIndex].mWeight, bone->mWeights[weightIndex].mVertexId });
			}
		}

		skinClusterData_.push_back(std::move(newMap));
		// nodeの解析
		rootNode_ = ReadNode(scene->mRootNode, scene);
		meshVertices.push_back(vertices);
	}

	// -------------------------------------------------
	// ↓ materialの解析
	// -------------------------------------------------
	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
		aiMaterial* material = scene->mMaterials[materialIndex];

		aiString materialName;
		if (AI_SUCCESS == material->Get(AI_MATKEY_NAME, materialName)) {
			std::string nameStr = materialName.C_Str();
			if (nameStr == "DefaultMaterial") {
				continue;
			}
		}

		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			materials.push_back(materialName.C_Str());
			materialData[materialName.C_Str()] = ModelMaterialData();
			std::string objTexture = textureFilePath.C_Str();
			materialData[materialName.C_Str()].textureFilePath = objTexture;
			TextureManager::LoadTextureFile(directoryPath, textureFilePath.C_Str());

			hasTexture_ = true;
		}
	}

	std::vector<std::unique_ptr<Mesh>> result;
	meshArray_.resize(meshVertices.size());
	for (uint32_t oi = 0; oi < meshVertices.size(); oi++) {
		/*mesh->SetUseMaterial(useMaterial[oi]);*/
		MeshManager::GetInstance()->AddMesh(device, fileName, meshVertices[oi], meshIndices[oi]);
		meshArray_ = (MeshManager::GetInstance()->GetMeshes(fileName));
		meshArray_[oi]->SetUseMaterial(useMaterial[oi]);
	}

	std::unordered_map<std::string, std::unique_ptr<Material>> materialResult;// 結果
	for (uint32_t oi = 0; oi < materials.size(); oi++) {
		/*materialResult[materials[oi]] = std::make_unique<Material>();
		materialResult[materials[oi]]->Init(device);
		materialResult[materials[oi]]->SetMaterialData(materialData[materials[oi]]);*/

		materialData_[materials[oi]] = materialData[materials[oi]];
	}

	//materialArray_ = std::move(materialResult);
}

Mesh* Model::GetMesh(const uint32_t& index) {
	return meshArray_[index].get();
}
