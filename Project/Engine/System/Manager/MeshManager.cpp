#include "MeshManager.h"
#include <unordered_map>

std::unordered_map<std::string, MeshManager::MeshArray> MeshManager::meshMap_;
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

void MeshManager::AddMesh(ID3D12Device* device, const std::string& modelName, const std::string& meshName,
						  const std::vector<Mesh::VertexData>& vertexData, std::vector<uint32_t>& indices) {

	// すでに存在していれば追加しない
	if (meshMap_.find(modelName) != meshMap_.end()) return;

	MeshPair meshPair(meshName, std::make_unique<Mesh>());
	meshPair.mesh->Init(device, vertexData, indices);

	// メッシュを登録
	meshMap_[modelName].meshArray.push_back(std::move(meshPair));

	// 名前リストを更新
	meshNameList_.push_back(modelName);
		meshMap_[modelName].push_back(std::move(mesh));
	}
}

std::vector<std::unique_ptr<Mesh>> MeshManager::GetMeshes(const std::string& modelName) {
	std::vector<std::unique_ptr<Mesh>> result;

	auto it = meshMap_.find(modelName);
	if (it == meshMap_.end()) {
		assert(false && "Model not found!");
	}

	for (auto& origine : it->second.meshArray) {
		result.emplace_back(std::make_unique<Mesh>(*origine.mesh));
	}

	return result;
}

bool MeshManager::ExistMesh(const std::string& modelName) {
	if (meshMap_.find(modelName) == meshMap_.end()) {
		return false;
	} else {
		return true;
	}
}
