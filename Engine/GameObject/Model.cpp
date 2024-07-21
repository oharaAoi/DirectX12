#include "Model.h"

Model::Model() {
}

Model::~Model() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 初期化関数
//////////////////////////////////////////////////////////////////////////////////////////////////
void Model::Init(ID3D12Device* device, const std::string& directorPath, const std::string& fileName) {
	std::string path = directorPath + "/" + fileName;

	materialArray_ = LoadMaterialData(directorPath, fileName, device);
	meshArray_ = LoadVertexData(path, device);

	Log("Load: " + fileName + "\n");

	//LoadObj(directorPath, fileName, device);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 更新関数
//////////////////////////////////////////////////////////////////////////////////////////////////
void Model::Update() {
	
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

		UINT size = meshArray_[oi]->GetVertexSize() / sizeof(Mesh::VertexData);

		commandList->DrawIndexedInstanced(size, 1, 0, 0, 0);
	}
}

void Model::ImGuiDraw(const std::string& name) {
	if (ImGui::TreeNode(name.c_str())) {
		for (uint32_t oi = 0; oi < meshArray_.size(); oi++) {
			materialArray_[meshArray_[oi]->GetUseMaterial()]->ImGuiDraw();
		}
		ImGui::TreePop();
	}
}

void Model::SetMaterials(const float& roughness, const float& metallic) {
	for (uint32_t oi = 0; oi < meshArray_.size(); oi++) {
		materialArray_[meshArray_[oi]->GetUseMaterial()]->SetMaterialParameter(roughness, metallic);
	}
}

/// <summary>
/// assimpでのNode解析
/// </summary>
/// <param name="node"></param>
/// <returns></returns>
Model::Node Model::ReadNode(aiNode* node) {
	Node result;
	aiMatrix4x4 aiLocalMat = node->mTransformation; // nodeのlocalMatrixを取得
	aiLocalMat.Transpose(); // 列ベクトル形式を行ベクトル形式に転置
	for (uint8_t row = 0; row < 4; row++) {
		for (uint8_t col = 0; col < 4; col++) {
			result.localMatrix.m[row][col] = aiLocalMat[row][col];
		}
	}
	result.name = node->mName.C_Str(); // Nodeの名前を格納
	result.children.resize(node->mNumChildren); // 子供の数だけ確保
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
		// 再帰的に読んで階層構造を作っていく
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}

	return result;
}

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
		mesh->Init(device, static_cast<uint32_t>(meshVertices[oi].size()) * sizeof(Mesh::VertexData), static_cast<uint32_t>(meshVertices[oi].size()));
		// 入れるMeshを初期化する
		mesh->SetUseMaterial(useMaterial[oi]);
		mesh->SetVertexData(meshVertices[oi]);
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
	std::ifstream file(directoryPath + "/" + fileName);
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
	std::ifstream file2(directoryPath + "/" + mtlFile);
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

void Model::LoadObj(const std::string& directoryPath, const std::string& fileName, ID3D12Device* device) {
	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + fileName;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes()); // meshがないのは対応しない

	std::vector<std::vector<Mesh::VertexData>> meshVertices;
	std::vector<std::string> useMaterial;
	std::vector<Mesh::VertexData> triangle;

	std::unordered_map<std::string, Material::ModelMaterialData> materialData;
	std::vector<std::string> materials;

	// meshの解析
	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals()); // 法線がないなら非対応
		assert(mesh->HasTextureCoords(0)); // texcoordがないmeshは非対応

		// faceの解析をする
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];

			assert(face.mNumIndices == 3); // 三角形のみ対応

			// vertexの解析を行う
			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
				
				Mesh::VertexData vertex;
				vertex.pos = {-position.x, position.y, position.z, 1.0f };
				vertex.normal = { -normal.x, normal.y, normal.z };
				vertex.texcoord = { texcoord.x, texcoord.y };

				triangle.push_back(vertex);
			}
		}

		// materialの解析
		for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
			aiMaterial* material = scene->mMaterials[materialIndex];

			aiString materialName;
			if (AI_SUCCESS == material->Get(AI_MATKEY_NAME, materialName)) {
				useMaterial.push_back(materialName.C_Str());
			}

			if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
				aiString textureFilePath;
				material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
				materials.push_back(materialName.C_Str());
				materialData[materialName.C_Str()] = Material::ModelMaterialData();
				materialData[materialName.C_Str()].textureFilePath = directoryPath + "/" + textureFilePath.C_Str();

				hasTexture_ = true;
			}
		}

		// nodeの解析
		rootNode_ = ReadNode(scene->mRootNode);
	}

	meshVertices.push_back(triangle);

	std::vector<std::unique_ptr<Mesh>> result;
	for (uint32_t oi = 0; oi < meshVertices.size(); oi++) {
		// Meshクラスの宣言
		std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>();
		mesh->Init(device, static_cast<uint32_t>(meshVertices[oi].size()) * sizeof(Mesh::VertexData), static_cast<uint32_t>(meshVertices[oi].size()));
		// 入れるMeshを初期化する
		mesh->SetUseMaterial(useMaterial[oi + 1]);
		mesh->SetVertexData(meshVertices[oi]);
		// Meshを配列に格納
		result.push_back(std::move(mesh));
	}

	std::unordered_map<std::string, std::unique_ptr<Material>> materialResult;// 結果
	for (uint32_t oi = 0; oi < materials.size(); oi++) {
		materialResult[materials[oi]] = std::make_unique<Material>();
		materialResult[materials[oi]]->Init(device);
		materialResult[materials[oi]]->SetMaterialData(materialData[materials[oi]]);
	}

	meshArray_ = std::move(result);
	materialArray_ = std::move(materialResult);
}
