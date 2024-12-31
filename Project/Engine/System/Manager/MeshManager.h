#pragma once
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include "Engine/Assets/Meshes/Mesh.h"

class MeshManager {
public:

	struct MeshPair {
		std::string meshName;
		std::unique_ptr<Mesh> mesh;

		MeshPair(const std::string& name, std::unique_ptr<Mesh> pMesh) {
			meshName = name;
			mesh = std::move(pMesh);
		}
	};

	struct MeshArray {
		std::vector<MeshPair> meshArray;
	};

public:

	MeshManager() = default;
	~MeshManager();
	MeshManager(const MeshManager&) = delete;
	const MeshManager& operator=(const MeshManager&) = delete;

	static MeshManager* GetInstance();

	void Init();
	void Finalize();

	static void AddMesh(ID3D12Device* device, const std::string& modelName,
						const std::string& meshName, const std::vector<Mesh::VertexData>& vertexData, std::vector<uint32_t>& indices);

	static std::vector<std::unique_ptr<Mesh>> GetMeshes(const std::string& modelName);

	bool ExistMesh(const std::string& modelName);

private:

	static std::unordered_map<std::string, MeshArray> meshMap_;
	static std::vector<std::string> meshNameList_;

};

