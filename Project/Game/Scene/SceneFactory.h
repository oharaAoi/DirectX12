#pragma once
#include <unordered_map>
#include "Engine/Lib/AbstractSceneFactory.h"
#include "Game/Scene/SceneType.h"

/// <summary>
/// シーン工場
/// </summary>
class SceneFactory : public AbstractSceneFactory {
public:

	SceneFactory() {};
	~SceneFactory() {};

	/// <summary>
	/// シーンの生成
	/// </summary>
	/// <param name="sceneName">: sceneの名前</param>
	/// <returns>: 生成したシーン</returns>
	std::unique_ptr<BaseScene> CreateScene(const std::string& sceneName)override;

	/// <summary>
	/// 指定したtpyeのシーンに対応した文字列を返す
	/// </summary>
	/// <param name="type"></param>
	/// <returns></returns>
	std::string SceneTypeToString(SceneType type) {
		auto it = sceneTypeToStringMap_.find(type);
		if (it != sceneTypeToStringMap_.end()) {
			return it->second;
		}
		return "Unknown";
	}

private:

	const std::unordered_map<SceneType, std::string> sceneTypeToStringMap_ = {
	{SceneType::TITLE, "TITLE"},
	{SceneType::GAME, "GAME"},
	{SceneType::TEST, "TEST"},
	};

};

