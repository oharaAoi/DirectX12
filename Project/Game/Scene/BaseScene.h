#pragma once
#include <optional>
#include "Engine/Manager/ModelManager.h"

enum SceneType {
	Scene_Game,
	Scene_Result,
	Scene_Test
};

/// <summary>
/// BaseとなるScene
/// </summary>
class BaseScene {
public:

	BaseScene() = default;
	virtual ~BaseScene() = default;

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() const = 0;

	void SetNextScene(const std::optional<SceneType>& sceneType) { type_ = sceneType; }
	std::optional<SceneType> GetType() const { return type_; }

protected:

	bool isPause_ = false;
	bool isStepFrame_ = false;

	std::optional<SceneType> type_ = std::nullopt;

};

