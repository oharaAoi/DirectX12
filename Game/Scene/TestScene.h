#pragma once
#include "Engine.h"
#include "Engine/Input/Input.h"
#include "Game/Scene/BaseScene.h"
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

class TestScene 
: public BaseScene {
public:


public:

	TestScene();
	~TestScene() override;

	void Init() override;
	void Update() override;
	void Draw() const override;

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

