#include "GameResources.h"
#include "Engine/Manager/TextureManager.h"
#include "Engine/Manager/ModelManager.h"
#include "Engine/Manager/AudioManager.h"

GameResources::GameResources() {
}

GameResources::~GameResources() {
}

void GameResources::Load() {
	/*ModelManager::LoadModel("./Engine/Resources/Develop/", "test.fbx");
	ModelManager::LoadModel("./Engine/Resources/Animation/", "amimationCharacter.fbx");*/

	LoadResourcesData(engineDirectoryPath_);

	// worldObject
	ModelManager::LoadModel("./Game/Resources/Model/Skydome/", "skydome.obj");
	ModelManager::LoadModel("./Game/Resources/Model/BackGround/", "BackGround.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Field/", "kair_field.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Field/", "kari_tutorialField.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Field/", "wall.obj");
	
	// gameObject
	ModelManager::LoadModel("./Game/Resources/Model/Boss_Body/", "boss_Body.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/Boss_Core/", "boss_core.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Boss_Eye/", "boss_eye.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Left_Hand/", "Left_Hand.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/Right_Hand/", "Right_Hand.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Player/", "wire.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/Barrier/", "barrier.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Missile/", "missile.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Bomb/", "bomb.obj");

	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "star.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "kari_bossHp.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "kari_fallUI.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "blackPanel.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "point.png");

	TextureManager::LoadTextureFile("./Game/Resources/Sprite/Missile/", "missileHitPoint.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/Missile/", "missileHitPointGauge.png");



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
