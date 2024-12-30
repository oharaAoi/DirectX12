#include "MeshManager.h"

std::unordered_map<std::string, std::vector<std::shared_ptr<Mesh>>> MeshManager::meshMap_;
std::vector<std::string> MeshManager::meshNameList_;

MeshManager::~MeshManager() {
}

MeshManager* MeshManager::GetInstance() {
	static MeshManager instance;
	return &instance;
}

void MeshManager::Init() {

}

void MeshManager::Finalize() {
	meshMap_.clear();
}

void MeshManager::AddMesh(ID3D12Device* device, const std::string& modelName,
						  const std::vector<Mesh::VertexData>& vertexData, std::vector<uint32_t>& indices) {
	// 同じ名前がすでに存在しない場合のみ追加
	if (meshMap_.find(modelName) == meshMap_.end()) {
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
		mesh->Init(device, vertexData, indices);
		meshNameList_.push_back(modelName);
		meshMap_[modelName].push_back(std::move(mesh));
	}
}

std::vector<std::shared_ptr<Mesh>> MeshManager::GetMeshes(const std::string& meshName) {
	auto it = meshMap_.find(meshName);
	if (it == meshMap_.end()) {
		assert(false && "Model not found!");
	}
	return it->second;
}
