#pragma once
#include "Engine.h"
// lib
#include "Transform.h"
#include "DrawUtils.h"
// gameObject
#include "Camera.h"
#include "Particle.h"
#include "Emitter.h"
#include "ParticleField.h"

class GameScene {
public:

	GameScene();
	~GameScene();

	void Init();

	void Update();

	void Draw();

private:

	std::unique_ptr<Camera> camera_ = nullptr;
	std::unique_ptr<Sphere> sphere_ = nullptr;
	std::unique_ptr<Particle> particle_ = nullptr;

	std::unique_ptr<Emitter> emitter_ = nullptr;
	std::unique_ptr<ParticleField> particleField_ = nullptr;

	// ---------- model ---------- //
	std::unique_ptr<Model> model_ = nullptr;
	std::unique_ptr<Model> sphereModel_ = nullptr;
	std::unique_ptr<Model> teapotModel_ = nullptr;

	std::unique_ptr<Model> terrainModel_ = nullptr;

	std::unique_ptr<Model> particleModel_ = nullptr;

	// ---------- vertex ---------- //
	Mesh::Vertices vertex_ = {
		{-0.5f, -0.5f, 0.0f, 1.0f},
		{ 0.0f, 0.5f, 0.0f, 1.0f },
		{ 0.5f, -0.5f, 0.0f, 1.0f }
	};

	Mesh::Vertices vertex2_ = {
		{-0.5f, -0.5f, 0.5f, 1.0f},
		{ 0.0f, 0.0f, 0.0f, 1.0f },
		{ 0.5f, -0.5f, -0.5f, 1.0f }
	};

	Mesh::RectVetices rect = {
		{0.0f, 0.0f, 0.0f, 1.0f},
		{640.0f, 0.0f, 0.0f, 1.0f},
		{0.0f, 360.0f, 0.0f, 1.0f},
		{640.0f, 360.0f, 0.0f, 1.0f}
	};

	// ---------- transform ---------- //
	kTransform transform_;
	kTransform sphereTransform_;
	kTransform sphereModelTransform_;
	kTransform teapotTransform_;

	// ---------- parameter ---------- //
	float roughness_;
	float metallic_;

	int lightKind_;
};
