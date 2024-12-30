#include "GameResources.h"
#include "Engine/System/Manager/TextureManager.h"
#include "Engine/System/Manager/ModelManager.h"
#include "Engine/System/Manager/AudioManager.h"

GameResources::GameResources() {
}

GameResources::~GameResources() {
}

void GameResources::Load() {
	/*ModelManager::LoadModel("./Engine/Resources/Develop/", "test.fbx");
	ModelManager::LoadModel("./Engine/Resources/Animation/", "amimationCharacter.fbx");*/

	LoadResourcesData(engineDirectoryPath_);
	LoadResourcesData(gameDirectoryPath_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　フォルダに入っているjsonファイルをすべて読む
//////////////////////////////////////////////////////////////////////////////////////////////////
void GameResources::LoadResourcesData(const std::string& directoryPath) {
	for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
		std::string fileName = entry.path().string();
		LoadResources(fileName);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　jsonファイルに入っているパスをすべて読む
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameResources::LoadResources(const std::string& filePath) {
	std::ifstream inputFile(filePath);
	if (!inputFile.is_open()) {
		std::string assertString = "not open" + filePath;
		assert(assertString.c_str());
		return;
	}

	// JSONファイルを読み込む
	json jsonData;
	inputFile >> jsonData;

	for (auto it = jsonData.begin(); it != jsonData.end(); ++it) {
		const std::string& category = it.key();		// データの種類
		const auto& groups = it.value();			// categoryに対応するデータ

		// ファイルの中を読み込む
		for (const auto& modelGroup : groups) {
			std::string directory = modelGroup["directory"];
			for (const auto& filename : modelGroup["files"]) {
				std::string file = filename;
				if (category == "models") {
					ModelManager::LoadModel(directory, file);
				} else if (category == "sprites") {
					TextureManager::LoadTextureFile(directory, file);
				}
			}
		}

	}
}
