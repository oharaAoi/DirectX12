#pragma once
#include "Engine.h"
// lib
#include "Transform.h"
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

	std::unique_ptr<Triangle> triangle_ = nullptr;
	std::unique_ptr<Triangle> triangle2_ = nullptr;

	std::unique_ptr<Sprite> sprite_ = nullptr;

	std::unique_ptr<Sphere> sphere_ = nullptr;

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

	kTransform transform_;

};
