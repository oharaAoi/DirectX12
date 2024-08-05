#pragma once
#include "Engine.h"
#include "Input.h"
// lib
#include "Transform.h"
#include "DrawUtils.h"
// gameObject
#include "BaseGameObject.h"
#include "Camera.h"
#include "Emitter.h"
#include "ParticleField.h"
// effectSystem
#include "EffectSystem.h"

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

