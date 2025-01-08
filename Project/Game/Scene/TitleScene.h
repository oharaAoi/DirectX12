#pragma once
// C++
#include <vector>
// Engine
#include "Engine.h"
#include "Engine/Assets/2d/Sprite.h"
#include "Engine/System/Audio/AudioPlayer.h"
// Game
#include "Game/Scene/BaseScene.h"
#include "Game/Camera/DebugCamera.h"
#include "Game/Camera/Camera3d.h"
#include "Game/WorldObject/Ground.h"
#include "Game/WorldObject/Skydome.h"
#include "Game/UI/TitlteUI.h"
#include "Game/UI/FadePanel.h"

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
	std::unique_ptr<Camera3d> camera_ = nullptr;

	// ------------------- worldObject ------------------- //
	std::unique_ptr<Ground> ground_ = nullptr;
	std::unique_ptr<Skydome> skydome_ = nullptr;

	// ------------------- Sprite ------------------- //

	std::unique_ptr<TitlteUI> titleUI_;
	std::unique_ptr<FadePanel> panel_;

	// ------------------- Audio ------------------- //

	std::unique_ptr<AudioPlayer> bgm_;

	// ------------------- parameter ------------------- //
	bool isDebug_ = false;
	bool isNexScene_ = false;

};

