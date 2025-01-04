#pragma once
// C++
#include <vector>
// Engine
#include "Engine.h"
// Game
#include "Game/Scene/BaseScene.h"
#include "Game/Camera/DebugCamera.h"
#include "Game/Camera/Camera.h"
#include "Game/WorldObject/Ground.h"
#include "Game/WorldObject/Skydome.h"

class TitleScene 
	: public BaseScene {
public:

	TitleScene();
	~TitleScene();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

#ifdef _DEBUG
	void Debug_Gui() override;
#endif

private:

	// ------------------- camera ------------------- //
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;
	std::unique_ptr<Camera> camera_ = nullptr;

	// ------------------- worldObject ------------------- //
	std::unique_ptr<Ground> ground_ = nullptr;
	std::unique_ptr<Skydome> skydome_ = nullptr;

	// ------------------- parameter ------------------- //
	bool isDebug_ = false;

};

