#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "Engine/GameObject/Model.h"

/// <summary>
/// Modelを管理するクラス
/// </summary>
class ModelManager {
public:

	ModelManager();
	~ModelManager();

	void Init();
	static void LoadOBJ(const std::string& directoryPath, const std::string& modelName);

private:

	static std::unordered_map<std::string, std::unique_ptr<Model>> modelMap_;

};

