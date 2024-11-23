#include "SceneFactory.h"

std::unique_ptr<BaseScene> SceneFactory::CreateScene(const std::string& sceneName) {
	// 次のシーンを作成
	std::unique_ptr<BaseScene> newScene;

	if (sceneName == SceneTypeToString(SceneType::TITLE)) {
		newScene = std::make_unique<TitleScene>();
	} else if (sceneName == SceneTypeToString(SceneType::GAME)) {
		newScene = std::make_unique<GameScene>();
	} else if (sceneName == SceneTypeToString(SceneType::GAMECLEAR)) {
		newScene = std::make_unique<GameClearScene>();
	} else if(sceneName == SceneTypeToString(SceneType::GAMEOVER))
		newScene = std::make_unique<GameOverScene>();
	else if(sceneName == SceneTypeToString(SceneType::TEST)) {
		newScene = std::make_unique<TestScene>();
	}

	return newScene;
}