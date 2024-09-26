#include "Loader.h"

std::vector<std::unique_ptr<Mesh>> LoadVertexData(const std::string& filePath, ID3D12Device* device) {
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
				Mesh::VertexData vertex{};

				position = positions[elementIndices[0] - 1];
				texcoord = texcoords[elementIndices[1] - 1];
				normal = normals[elementIndices[2] - 1];
				vertex = { position, texcoord, normal };

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

std::unordered_map<std::string, std::unique_ptr<Material>> LoadMaterialData(const std::string& directoryPath, const std::string& fileName, ID3D12Device* device) {
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
			materialDatas[materialName].textureFilePath = textureFilename;
			TextureManager::LoadTextureFile(directoryPath, textureFilename);

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
