#pragma once
#include "Engine.h"
#include "Game/Scene/BaseScene.h"
// lib
#include "Engine/Lib/Transform.h"
#include "Engine/Utilities/DrawUtils.h"
// gameObject
#include "Game/Camera/Camera.h"

class GameScene 
	: public BaseScene {
public:

	GameScene();
	~GameScene();

	void Init() override;
	void Load()  override;
	void Update() override;
	void Draw() const override;

private:

	std::unique_ptr<Camera> camera_ = nullptr;
	std::unique_ptr<Sphere> sphere_ = nullptr;
	
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

	// ---------- parameter ---------- //
	float roughness_;
	float metallic_;

	int lightKind_;

	bool isDraw_;

	// ---------- sound ---------- //
	SeData soundData_;
	BgmData bgmData_;
};
