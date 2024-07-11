#pragma once
#include "Engine.h"
#include "Input.h"
// lib
#include "Transform.h"
#include "DrawUtils.h"
// gameObject
#include "BaseGameObject.h"
#include "Camera.h"
#include "Particle.h"
#include "Emitter.h"
#include "ParticleField.h"

class TestScene {
public:


public:

	TestScene();
	~TestScene();

	void Init();

	void Update();

	void Draw();

	void AddGameObject();

private:

	// カメラ ----------------------------------------------
	std::unique_ptr<Camera> camera_ = nullptr;

	// モデル ----------------------------------------------
	std::unique_ptr<Model> skinModel_ = nullptr;

	// トランスフォーム --------------------------------------
	WorldTransform skinTransform_;

	// ゲームオブジェクト ------------------------------------
	int objectKind_;
};

