#include "Model.h"

Model::Model() {
}

Model::~Model() {

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 初期化関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void Model::Init(ID3D12Device* device, const std::string& directorPath, const std::string& fileName) {
	//materialArray_ w= LoadMaterialData(directorPath, fileName, device);
	//meshArray_ = LoadVertexData(path, device);

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

	//AnimationUpdate();
	/*Matrix4x4 mat = MakeAffineMatrix(localTransform_);
	rootNode_.localMatrix = mat;*/
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 描画関数
//////////////////////////////////////////////////////////////////////////////////////////////////
void Model::Draw(ID3D12GraphicsCommandList* commandList, const WorldTransform& worldTransform, const ViewProjection* viewProjection) {
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (uint32_t oi = 0; oi < meshArray_.size(); oi++) {
		meshArray_[oi]->Draw(commandList);
		materialArray_[meshArray_[oi]->GetUseMaterial()]->Draw(commandList);

		worldTransform.Draw(commandList);
		viewProjection->Draw(commandList);

		if (hasTexture_) {
			std::string textureName = materialArray_[meshArray_[oi]->GetUseMaterial()]->GetMateriaData().textureFilePath;
			TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, textureName, 3);
		}

		commandList->DrawIndexedInstanced(meshArray_[oi]->GetIndexNum(), 1, 0, 0, 0);
	}
}

void Model::DrawSkinning(ID3D12GraphicsCommandList* commandList, const Skinning* skinning, const WorldTransform& worldTransform, const ViewProjection* viewprojection) {
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (uint32_t oi = 0; oi < meshArray_.size(); oi++) {
		skinning->StackCommand(commandList, meshArray_[oi]->GetVBV());
		meshArray_[oi]->DrawIndex(commandList);
		materialArray_[meshArray_[oi]->GetUseMaterial()]->Draw(commandList);

		worldTransform.Draw(commandList);
		viewprojection->Draw(commandList);

		if (hasTexture_) {
			std::string textureName = materialArray_[meshArray_[oi]->GetUseMaterial()]->GetMateriaData().textureFilePath;
			TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, textureName, 3);
		}

		commandList->DrawIndexedInstanced(meshArray_[oi]->GetIndexNum(), 1, 0, 0, 0);
	}
}

void Model::ImGuiDraw(const std::string& name) {
	if (ImGui::TreeNode(name.c_str())) {
		for (uint32_t oi = 0; oi < meshArray_.size(); oi++) {
			std::string materialNum = std::to_string(oi);
			std::string materialName = "material" + materialNum;
			if (ImGui::TreeNode(materialName.c_str())) {
				materialArray_[meshArray_[oi]->GetUseMaterial()]->ImGuiDraw();
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
}

/// <summary>
/// assimpでのNode解析
/// </summary>
/// <param name="node"></param>
/// <returns></returns>
Model::Node Model::ReadNode(aiNode* node, const aiScene* scene) {
	Node result;
	// ----------------------------------
	// LocalMatrixを取得する
	// ----------------------------------
	aiVector3D scale, translate;
	aiQuaternion rotate;
	node->mTransformation.Decompose(scale, rotate, translate);
	result.transform.scale = { scale.x, scale.y, scale.z };
	result.transform.rotate = { rotate.x, -rotate.y, -rotate.z, rotate.w };
	result.transform.translate = { -translate.x, translate.y, translate.z };
	result.localMatrix = MakeAffineMatrix(result.transform.scale, result.transform.rotate, result.transform.translate);
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


void Model::LoadObj(const std::string& directoryPath, const std::string& fileName, ID3D12Device* device) {
	Assimp::Importer importer;
	std::string filePath = directoryPath + fileName;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs |
											 aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);
	assert(scene->HasMeshes()); // meshがないのは対応しない

	std::vector<std::vector<Mesh::VertexData>> meshVertices;
	std::vector<std::vector<uint32_t>> meshIndices;
	std::vector<std::string> useMaterial;

	std::unordered_map<std::string, Material::ModelMaterialData> materialData;
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
				meshIndices[scene->mNumMeshes - 1].push_back(vertexIndex);
			}
			/*for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[face.mNumIndices - 1 - element];
				meshIndices[scene->mNumMeshes - 1].push_back(vertexIndex);
			}*/
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
		for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {
			// jointごとの格納領域を作る
			aiBone* bone = mesh->mBones[boneIndex];
			std::string jointName = bone->mName.C_Str();
			Skinning::JointWeightData& jointWeightData = skinClusterData_[jointName];

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
			materialData[materialName.C_Str()] = Material::ModelMaterialData();
			std::string objTexture = textureFilePath.C_Str();
			materialData[materialName.C_Str()].textureFilePath = objTexture;
			TextureManager::LoadTextureFile(directoryPath, textureFilePath.C_Str());

			hasTexture_ = true;
		}
	}

	std::vector<std::unique_ptr<Mesh>> result;
	for (uint32_t oi = 0; oi < meshVertices.size(); oi++) {
		// Meshクラスの宣言
		std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>();
		mesh->Init(device, meshVertices[oi], meshIndices[oi]);
		// 入れるMeshを初期化する(直すところ)
		mesh->SetUseMaterial(useMaterial[oi]);
		// Meshを配列に格納
		meshArray_.push_back(std::move(mesh));
	}

	std::unordered_map<std::string, std::unique_ptr<Material>> materialResult;// 結果
	for (uint32_t oi = 0; oi < materials.size(); oi++) {
		materialResult[materials[oi]] = std::make_unique<Material>();
		materialResult[materials[oi]]->Init(device);
		materialResult[materials[oi]]->SetMaterialData(materialData[materials[oi]]);
	}

	materialArray_ = std::move(materialResult);
}


void Model::AnimationUpdate() {
	for (auto& anim : rootNode_.animationsData.animations) {
		aiVector3D vectorPos = InterpolationPosition(anim, currentAnimationTime_);
		aiVector3D vectorRotation = InterpolationRotation(anim, currentAnimationTime_);

		localTransform_.translate = { vectorPos.x,  vectorPos.y,  vectorPos.z };
		localTransform_.rotate = { vectorRotation.x,  vectorRotation.y, vectorRotation.z };
	}
}

//void Model::LoadAnimation(const std::string& directoryPath, const std::string& fileName) {
//	Assimp::Importer importer;
//	std::string filePath = directoryPath + fileName;
//	const aiScene* scene = importer.ReadFile(filePath.c_str(), 0);
//
//	for (uint32_t animIndex = 0; animIndex < scene->mNumAnimations; ++animIndex) {
//		// ----------------------------------
//		// Animationを取得する
//		// ----------------------------------
//		aiAnimation* anime = scene->mAnimations[animIndex];
//		for (uint32_t channelIndex = 0; channelIndex < anime->mNumChannels; ++channelIndex) {
//			aiNodeAnim* nodeAnim = anime->mChannels[channelIndex];
//			if (nodeAnim->mNodeName == scene->mRootNode->mName) {
//				NodeAnimationData data;
//				data.animations.push_back(NodeAnimation(nodeAnim));
//				data.tickPerSecond = static_cast<float>(anime->mTicksPerSecond);
//				data.duration = static_cast<float>(anime->mDuration);
//				// データを入れる
//				rootNode_.animationsData = data;
//			}
//		}
//	}
//}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Meshを読み込む関数
//////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<std::unique_ptr<Mesh>> Model::LoadVertexData(const std::string& filePath, ID3D12Device* device) {
	// Meshを作成
	std::vector<std::unique_ptr<Mesh>> result;
	std::vector<Mesh::VertexData> currentVertices;
	// 複数のMeshを格納
	std::vector<std::vector<Mesh::VertexData>> meshVertices;

	std::vector<Vector4> positions; // 位置
	std::vector<Vector3> normals;// 法線
	std::vector<Vector2> texcoords;// テクスチャ座標
	std::string line;// ファイルから読み込んだ1行を格納する物
	std::vector<std::string> useMaterial;

	// ファイルを開く
	std::ifstream file(filePath);
	assert(file.is_open());

	// Meshの情報を読む
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream ss(line);
		ss >> identifier; // 先頭の識別子を読む

		if (identifier == "o") {
			if (!currentVertices.empty()) {
				meshVertices.push_back(currentVertices);
			}

			currentVertices.clear();

		} else if (identifier == "usemtl") {
			std::string useName;
			ss >> useName;
			useMaterial.push_back(useName);

			// 頂点情報を読む
		} else if (identifier == "v") {
			Vector4 position;
			ss >> position.x >> position.y >> position.z;
			position.z *= -1.0f;
			position.w = 1.0f;
			//position.x *= -1.0f;
			positions.push_back(position);

		} else if (identifier == "vt") {
			Vector2 texcoord;
			ss >> texcoord.x >> texcoord.y;
			texcoord.y = 1.0f - texcoord.y;
			texcoords.push_back(texcoord);

		} else if (identifier == "vn") {
			// 法線
			Vector3 normal;
			ss >> normal.x >> normal.y >> normal.z;
			normal.z *= -1.0f;
			//normal.x *= -1.0f;
			normals.push_back(normal);

		} else if (identifier == "f") {
			Mesh::VertexData triangle[3];

			// 面は三角形固定。その他は未対応
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				ss >> vertexDefinition;
				// 頂点の要素へのindexが[位置/UV/法線]で格納されているので分解ぢてindexを取得する
				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3];

				for (uint32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/'); // /区切りでインデックスを読んでいく
					if (index == "") {
						index = "0";
					}
					elementIndices[element] = std::stoi(index);
				}

				// 要素へのindexから、実際の要素を取得して頂点を構築する
				Vector4 position{};
				Vector2 texcoord{};
				Vector3 normal{};
				/*Mesh::VertexData vertex{};*/

				if (hasTexture_) {
					position = positions[elementIndices[0] - 1];
					texcoord = texcoords[elementIndices[1] - 1];
					normal = normals[elementIndices[2] - 1];
					/*vertex = { position, texcoord, normal };*/
				} else {
					position = positions[elementIndices[0] - 1];
					texcoord = { 0,0 };
					normal = normals[elementIndices[2] - 1];
					/*vertex = { position, texcoord, normal };*/
				}

				triangle[faceVertex] = { position, texcoord, normal };
			}

			currentVertices.push_back(triangle[2]);
			currentVertices.push_back(triangle[1]);
			currentVertices.push_back(triangle[0]);

			Mesh::CalculateTangent(triangle);
		}
	}

	// 最後のデータを格納する
	if (!currentVertices.empty()) {
		meshVertices.push_back(currentVertices);
	}

	for (uint32_t oi = 0; oi < meshVertices.size(); oi++) {
		// Meshクラスの宣言
		std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>();
		//mesh->Init(device, meshVertices[oi], meshVertices[oi]);
		// 入れるMeshを初期化する
		mesh->SetUseMaterial(useMaterial[oi]);
		// Meshを配列に格納
		result.push_back(std::move(mesh));
	}

	return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Materialを読み込む関数
//////////////////////////////////////////////////////////////////////////////////////////////////
std::unordered_map<std::string, std::unique_ptr<Material>> Model::LoadMaterialData(const std::string& directoryPath, const std::string& fileName, ID3D12Device* device) {
	std::unordered_map<std::string, std::unique_ptr<Material>> result;// 結果
	std::unordered_map<std::string, Material::ModelMaterialData> materialDatas;// 後で一気に結果の変数に代入するための物

	std::string line;// ファイルから読み込んだ1行を格納する物
	std::string materialName; // newmtlの名前

	// マテリアルの名前を格納しておく
	std::vector<std::string> materials;

	// mtlファイル
	std::string mtlFile;

	// objファイルからmtlファイルを探す
	std::ifstream file(directoryPath + fileName);
	assert(file.is_open());

	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier; // 先頭の識別子を読む

		if (identifier == "mtllib") {
			std::string mtlFileName;
			s >> mtlFileName;

			mtlFile = mtlFileName;
		}
	}

	// mtlファイルを開く
	std::ifstream file2(directoryPath + mtlFile);
	assert(file2.is_open());

	hasTexture_ = false;

	// ファイルを読む
	while (std::getline(file2, line)) {
		std::string materialIdentifier;
		std::istringstream s(line);
		s >> materialIdentifier; // 先頭の識別子を読む

		if (materialIdentifier == "newmtl") {
			s >> materialName;
			materials.push_back(materialName);
			materialDatas[materialName] = Material::ModelMaterialData();

		} else if (materialIdentifier == "map_Kd") {
			// テクスチャマップを読み取る
			std::string textureFilename;
			s >> textureFilename;
			materialDatas[materialName].textureFilePath = directoryPath + "/" + textureFilename;

			hasTexture_ = true;

		} else if (materialIdentifier == "Ka") {
			// アルベド色を読み取る(環境反射率)
			Vector4 color;
			s >> color.x >> color.y >> color.z;
			materialDatas[materialName].albedo = Vector4(color.x, color.y, color.z, 1.0f);
		} else if (materialIdentifier == "Kd") {
			// ディフューズ色を読み取る(拡散反射率)
			Vector4 color;
			s >> color.x >> color.y >> color.z;
			materialDatas[materialName].diffuse = Vector4(color.x, color.y, color.z, 1.0f);
		} else if (materialIdentifier == "Ks") {
			// スペキュラ色(鏡面反射率)
			Vector4 color;
			s >> color.x >> color.y >> color.z;
			materialDatas[materialName].specular = Vector4(color.x, color.y, color.z, 1.0f);

		} else if (materialIdentifier == "Ke") {
			// 自己発光
			Vector4 color;
			s >> color.x >> color.y >> color.z;
			materialDatas[materialName].emissive = Vector4(color.x, color.y, color.z, 1.0f);

		} else if (materialIdentifier == "Ni") {
			// 屈折率
			float refraction;
			s >> refraction;
			materialDatas[materialName].refraction = refraction;

		} else if (materialIdentifier == "Ns") {
			// shininess(鏡面反射の鋭さ)
		}
	}

	for (uint32_t oi = 0; oi < materials.size(); oi++) {
		result[materials[oi]] = std::make_unique<Material>();
		result[materials[oi]]->Init(device);
		result[materials[oi]]->SetMaterialData(materialDatas[materials[oi]]);
	}

	return result;
}