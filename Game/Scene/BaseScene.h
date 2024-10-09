#pragma once
#include "Engine/Manager/ModelManager.h"

/// <summary>
/// BaseとなるScene
/// </summary>
class BaseScene {
public:

	BaseScene() = default;
	virtual ~BaseScene() = default;

	virtual void Finalize() = 0;
	virtual void Init() = 0;
	virtual void Load() = 0;
	virtual void Update() = 0;
	virtual void Draw() const = 0;

};

