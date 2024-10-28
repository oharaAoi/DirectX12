#include "Loader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

std::vector<std::unique_ptr<Mesh>> LoadVertexData(const std::string& directoryPath, const std::string& fileName, ID3D12Device* device) {
	Assimp::Importer importer;
	std::string filePath = directoryPath + fileName;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs | aiProcess_Triangulate | aiProcess_CalcTangentSpace);
	assert(scene->HasMeshes()); // meshがないのは対応しない

	std::vector<std::vector<Mesh::VertexData>> meshVertices;
	std::vector<std::vector<uint32_t>> meshIndices;
	std::vector<std::string> useMaterial;

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
		// nodeの解析
		meshVertices.push_back(vertices);
	}

	std::vector<std::unique_ptr<Mesh>> result;
	for (uint32_t oi = 0; oi < meshVertices.size(); oi++) {
		// Meshクラスの宣言
		std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>();
		mesh->Init(device, meshVertices[oi], meshIndices[oi]);
		// 入れるMeshを初期化する(直すところ)
		mesh->SetUseMaterial(useMaterial[oi]);
		// Meshを配列に格納
		result.push_back(std::move(mesh));
	}

	return result;
}

std::unordered_map<std::string, std::unique_ptr<Material>> LoadMaterialData(const std::string& directoryPath, const std::string& fileName, ID3D12Device* device) {
	Assimp::Importer importer;
	std::string filePath = directoryPath + fileName;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs | aiProcess_Triangulate | aiProcess_CalcTangentSpace);
	assert(scene->HasMeshes()); // meshがないのは対応しない

	std::unordered_map<std::string, Model::ModelMaterialData> materialData;
	std::vector<std::string> materials;
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
			materialData[materialName.C_Str()] = Model::ModelMaterialData();
			std::string objTexture = textureFilePath.C_Str();
			materialData[materialName.C_Str()].textureFilePath = objTexture;
			TextureManager::LoadTextureFile(directoryPath, textureFilePath.C_Str());
		}
	}

	std::unordered_map<std::string, std::unique_ptr<Material>> materialResult;// 結果
	for (uint32_t oi = 0; oi < materials.size(); oi++) {
		materialResult[materials[oi]] = std::make_unique<Material>();
		materialResult[materials[oi]]->Init(device);
		materialResult[materials[oi]]->SetMaterialData(materialData[materials[oi]]);
	}

	return materialResult;
}
