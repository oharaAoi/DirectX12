#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "Engine/Assets/Mesh.h"

class MeshManager {
public:

	MeshManager() = default;
	~MeshManager();
	MeshManager(const MeshManager&) = delete;
	const MeshManager& operator=(const MeshManager&) = delete;

	static MeshManager* GetInstance();

	void Init();
	void Finalize();

	static void AddMesh(ID3D12Device* device, const std::string& modelName, const std::vector<Mesh::VertexData>& vertexData, std::vector<uint32_t>& indices);

	static std::vector<std::shared_ptr<Mesh>> GetMeshes(const std::string& meshName);

private:

	static std::unordered_map<std::string, std::vector<std::shared_ptr<Mesh>>> meshMap_;
	static std::vector<std::string> meshNameList_;

};

