#pragma once
#include <string>
#include "Game/Scene/BaseScene.h"

/// <summary>
/// シーン工場
/// </summary>
class AbstractSceneFactory {
public:
	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~AbstractSceneFactory() = default;

	/// <summary>
	/// シーン生成
	/// </summary>
	/// <param name="sceneName">: sceneの名前</param>
	/// <returns>: scene</returns>
	virtual std::unique_ptr<BaseScene> CreateScene(const std::string& sceneName) = 0;
};

