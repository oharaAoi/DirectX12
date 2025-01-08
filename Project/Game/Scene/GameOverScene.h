#pragma once
// C++
#include <memory>
// Engine
#include "Engine.h"
// Game
#include "Game/Scene/BaseScene.h"
#include "Game/Camera/DebugCamera.h"
#include "Game/Camera/Camera3d.h"
#include "Game/UI/FadePanel.h"

class GameOverScene
	: public BaseScene {
public:

	GameOverScene();
	~GameOverScene();

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
	std::unique_ptr<Camera3d> camera_ = nullptr;

	std::unique_ptr<Sprite> clearUI_;
	std::unique_ptr<Sprite> clearBack_;
	
	// ------------------- parameter ------------------- //
	bool isDebug_ = false;
	bool isNexScene_ = false;

};

