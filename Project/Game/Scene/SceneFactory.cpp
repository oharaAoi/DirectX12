#include "SceneFactory.h"
#include "Game/Scene/TitleScene.h"
#include "Game/Scene/GameScene.h"
#include "Game/Scene/TestScene.h"

std::unique_ptr<BaseScene> SceneFactory::CreateScene(const std::string& sceneName) {
	// 次のシーンを作成
	std::unique_ptr<BaseScene> newScene;

	if (sceneName == SceneTypeToString(SceneType::TITLE)) {
		newScene = std::make_unique<TitleScene>();
	} else if (sceneName == SceneTypeToString(SceneType::GAME)) {
		newScene = std::make_unique<GameScene>();
	} else if (sceneName == SceneTypeToString(SceneType::TEST)) {
		newScene = std::make_unique<TestScene>();
	}

	return newScene;
}