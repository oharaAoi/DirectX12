#pragma once
#include "Engine.h"
#include "GameScene.h"

class GameScene {
public:

	GameScene();
	~GameScene();

	void Init();

	void Update();

	void Draw();

private:

	Mesh::Vertices vertex_ = {
		{-0.5f, -0.5f, 0.0f, 1.0f},
		{ 0.0f, 0.5f, 0.0f, 1.0f },
		{ 0.5f, -0.5f, 0.0f, 1.0f }
	};

	std::unique_ptr<Triangle> triangle_ = nullptr;

};
