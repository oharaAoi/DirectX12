#pragma once
#include "Engine.h"
#include "Input.h"
// lib
#include "Transform.h"
#include "DrawUtils.h"
// gameObject
#include "Camera.h"
#include "Particle.h"
#include "Emitter.h"
#include "ParticleField.h"

class TestScene {
public:

	TestScene();
	~TestScene();

	void Init();

	void Update();

	void Draw();

private:

	// カメラ ----------------------------------------------
	std::unique_ptr<Camera> camera_ = nullptr;

	// モデル ----------------------------------------------
	std::unique_ptr<Model> skinModel_ = nullptr;

	// トランスフォーム --------------------------------------
	kTransform skinTransform_;
};

