#pragma once
#include <optional>
#include "Engine/System/Manager/ModelManager.h"
#include "Game/Scene/SceneType.h"

/// <summary>
/// BaseとなるScene
/// </summary>
class BaseScene {
public:

	BaseScene() = default;
	virtual ~BaseScene() = default;

	virtual void Finalize() = 0;
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() const = 0;

#ifdef _DEBUG
	virtual void Debug_Gui() = 0;
#endif

	const std::optional<SceneType>& GetNextSceneType() const { return nextSceneType_; }
	void SetNextSceneType(const std::optional<SceneType>& type) { nextSceneType_ = type; }

protected:

	std::optional<SceneType> nextSceneType_ = std::nullopt;

};

