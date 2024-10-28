#include "ModelManager.h"

std::unordered_map<std::string, std::unique_ptr<Model>> ModelManager::modelMap_;
std::vector<std::string> ModelManager::modelNameList_;

ModelManager::~ModelManager() {}

ModelManager* ModelManager::GetInstance() {
	static ModelManager instance;
	return &instance;
}

void ModelManager::Init() {
	modelMap_.clear();
}

void ModelManager::Finalize() {
	modelMap_.clear();
}

void ModelManager::LoadModel(const std::string& directoryPath, const std::string& modelName) {
	if (auto it = modelMap_.find(modelName); it != modelMap_.end()) {
		return;
	}
	modelMap_.try_emplace(modelName, Engine::CreateModel(directoryPath, modelName));
}

Model* ModelManager::GetModel(const std::string& modelName) {
	if (auto it = modelMap_.find(modelName); it == modelMap_.end()) {
		assert(false && "Model not found!");
	}

	modelNameList_.push_back(modelName);
	return modelMap_[modelName].get();
}
