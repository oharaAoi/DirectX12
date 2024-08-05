#pragma once
#include "Engine.h"
#include "Input.h"
// lib
#include "Transform.h"
#include "DrawUtils.h"
// gameObject
#include "Camera.h"

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

	// トランスフォーム --------------------------------------
	WorldTransform sphereTransform_;
	WorldTransform sphereModelTransform_;

	// ---------- parameter ---------- //
	float roughness_;
	float metallic_;

	int lightKind_;

	// ---------- sound ---------- //
	SeData soundData_;
	BgmData bgmData_;
};
