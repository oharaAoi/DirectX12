#pragma once
#include "Engine.h"
#include "Engine/Input/Input.h"
// lib
#include "Engine/Lib/Transform.h"
#include "Engine/Utilities/DrawUtils.h"
// gameObject
#include "Engine/GameObject/BaseGameObject.h"
#include "Game/Camera/Camera.h"
#include "Engine/ParticleSystem/Emitter.h"
#include "Engine/ParticleSystem/ParticleField.h"
// effectSystem
#include "Engine/ParticleSystem/EffectSystem.h"

class TestScene {
public:


public:

	TestScene();
	~TestScene();

	void Init();

	void Update();

	void Draw();

	void ImGuiDraw();

private:

	// カメラ ----------------------------------------------
	std::unique_ptr<Camera> camera_ = nullptr;

	// モデル ----------------------------------------------
	std::unique_ptr<Model> skinModel_ = nullptr;
	std::unique_ptr<Model> sphereModel_ = nullptr;
	std::unique_ptr<Model> cubeModel_ = nullptr;

	// トランスフォーム --------------------------------------
	WorldTransform skinTransform_;
	WorldTransform sphereTransform_;
	WorldTransform cubeTransform_;

	// ゲームオブジェクト ------------------------------------
	int objectKind_;
};

