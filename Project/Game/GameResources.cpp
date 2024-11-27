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
	ModelManager::LoadModel("./Game/Resources/Model/Hook/", "hook.obj");
	
	// gameObject
	ModelManager::LoadModel("./Game/Resources/Model/Boss_Body/", "boss_Body.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/Boss_Core/", "boss_core.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Boss_Eye/", "boss_eye.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Left_Hand/", "Left_Hand.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/Right_Hand/", "Right_Hand.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Player/", "wire.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/Player/", "prediction.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Barrier/", "barrier.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Missile/", "missile.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Bomb/", "bomb.obj");
	ModelManager::LoadModel("./Game/Resources/Model/TitlePlayer/", "TitlePlayer.obj");
	ModelManager::LoadModel("./Game/Resources/Model/FallRock/", "FallRock.obj");
	ModelManager::LoadModel("./Game/Resources/Model/PlaneSphere/", "planeSphere.obj");

	// UI3D
	ModelManager::LoadModel("./Game/Resources/Model/BossHP/", "bossGauge.obj");
	ModelManager::LoadModel("./Game/Resources/Model/BossHP/", "bossHpOutSide.obj");
	ModelManager::LoadModel("./Game/Resources/Model/BossHP/", "bossGaugeOrigin.obj");
	ModelManager::LoadModel("./Game/Resources/Model/BossHP/", "bossHpOutSideOrigin.obj");

	// タイトルなどで使用
	ModelManager::LoadModel("./Game/Resources/Model/UI/", "retry.obj");
	ModelManager::LoadModel("./Game/Resources/Model/UI/", "title.obj");

	ModelManager::LoadModel("./Game/Resources/Model/PlayerHp/", "playerHpGauge.obj");
	ModelManager::LoadModel("./Game/Resources/Model/PlayerHp/", "playerHpOutSide.obj");

	// Effect
	ModelManager::LoadModel("./Game/Resources/Model/Hand_Effect/", "Hand_Effect.gltf");

	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "star.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "kari_bossHp.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "kari_fallUI.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "blackPanel.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "point.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "danger.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "clearUI.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "goTitle.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "dangerGauge.png");

	TextureManager::LoadTextureFile("./Game/Resources/Sprite/Missile/", "missileHitPoint.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/Missile/", "missileHitPointGauge.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/Missile/", "missileLockOn.png");

	// Tutorial
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/TutorialScene/", "tutorial_01.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/TutorialScene/", "tutorial_02.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/TutorialScene/", "tutorial_03.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/TutorialScene/", "tutorial_04.png");

	// BGM
	AudioManager::LoadAudio("./Game/Resources/Audio/BGM/","FIRST_Buttle.mp3");
	AudioManager::LoadAudio("./Game/Resources/Audio/BGM/","gameClear.mp3");
	AudioManager::LoadAudio("./Game/Resources/Audio/BGM/","SECOND_Buttle.mp3");
	AudioManager::LoadAudio("./Game/Resources/Audio/BGM/","title.mp3");
	AudioManager::LoadAudio("./Game/Resources/Audio/BGM/","turorial.mp3");
	AudioManager::LoadAudio("./Game/Resources/Audio/BGM/","gameOver.mp3");

	// SE
	AudioManager::LoadAudio("./Game/Resources/Audio/SE/", "bossStartUp.mp3");
	AudioManager::LoadAudio("./Game/Resources/Audio/SE/", "bossMove.mp3");
	AudioManager::LoadAudio("./Game/Resources/Audio/SE/", "playerHited.mp3");
	AudioManager::LoadAudio("./Game/Resources/Audio/SE/", "bossCoreHited.mp3");
	AudioManager::LoadAudio("./Game/Resources/Audio/SE/", "slap.mp3");
	AudioManager::LoadAudio("./Game/Resources/Audio/SE/", "objPull.mp3");
	AudioManager::LoadAudio("./Game/Resources/Audio/SE/", "fireMissile.mp3");
	AudioManager::LoadAudio("./Game/Resources/Audio/SE/", "mowDown.mp3");
	AudioManager::LoadAudio("./Game/Resources/Audio/SE/", "missileHited.mp3");
	AudioManager::LoadAudio("./Game/Resources/Audio/SE/", "barrierExpand.mp3");
	AudioManager::LoadAudio("./Game/Resources/Audio/SE/", "barrierBreak.mp3");
	AudioManager::LoadAudio("./Game/Resources/Audio/SE/", "strechWire.mp3");
	AudioManager::LoadAudio("./Game/Resources/Audio/SE/", "select.mp3");

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
