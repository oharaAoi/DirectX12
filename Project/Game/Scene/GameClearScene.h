#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include "Engine/2d/Sprite.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Game/Scene/BaseScene.h"
#include "Game/WorldObject/Skydome.h"
#include "Game/Camera/FollowCamera.h"
#include "Game/GameObject/Player/TitlePlayer.h"
#include "Game/UI/Panel.h"
#include "Engine/Audio/AudioPlayer.h"

class GameClearScene 
	: public BaseScene {
public:

	GameClearScene();
	~GameClearScene() override;

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void DrawUI();

	void AutoUpdate();
	void ClearUpdate();

#ifdef _DEBUG
	void ImGuiDraw();
#endif

	// --- 調整項目 ----------------------------------- //
	AdjustmentItem* adjustmentItem_;

	// --- eyePos/view/Projection ------------------- //
	Vector3 eyePos_;
	Matrix4x4 viewMat_;
	Matrix4x4 projectionMat_;

	// --- audio -------------------------------------- //
	std::unique_ptr<AudioPlayer> bgm_;


	std::unique_ptr<FollowCamera> followCamera_;

	std::unique_ptr<Skydome> skydome_;


	std::unique_ptr<TitlePlayer> player_;


	std::unique_ptr<Sprite> clearUI_;
	std::unique_ptr<Sprite> goTitle_;

	std::unique_ptr<Panel> panel_;
	bool isNextScene_ = false;

};
