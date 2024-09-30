#pragma once
#include "Engine.h"
#include "Engine/Input/Input.h"
#include "Game/Scene/BaseScene.h"
#include "Engine/Lib/Transform.h"
#include "Engine/Utilities/DrawUtils.h"
#include "Engine/GameObject/BaseGameObject.h"
#include "Game/Camera/Camera.h"
#include "Engine/ParticleSystem/Emitter.h"
#include "Engine/ParticleSystem/ParticleField.h"
#include "Engine/ParticleSystem/EffectSystem.h"

class TestScene 
: public BaseScene {
public:


public:

	TestScene();
	~TestScene() override;

	void Init() override;
	void Load() override;
	void Update() override;
	void Draw() const override;

#ifdef _DEBUG
	void ImGuiDraw();
#endif

private:

	// カメラ ----------------------------------------------
	std::unique_ptr<Camera> camera_ = nullptr;

	// ゲームオブジェクト ------------------------------------
	std::unique_ptr<BaseGameObject> testObj_ = nullptr;
	std::unique_ptr<Sprite> sprite_ = nullptr;
};

