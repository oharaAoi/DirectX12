#pragma once
#include <memory>
#include "Engine/Utilities/AdjustmentItem.h"
#include "Game/Scene/BaseScene.h"
#include "Game/WorldObject/Skydome.h"
#include "Game/Camera/FollowCamera.h"
#include "Game/GameObject/Player/TitlePlayer.h"
#include "Game/UI/Panel.h"
#include "Engine/Audio/AudioPlayer.h"

class TitleScene
	: public BaseScene {
public:

	TitleScene();
	~TitleScene() override;

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void AutoUpdate();
	void TitleUpdate();

#ifdef _DEBUG
	void ImGuiDraw();
#endif

private:

	// --- 調整項目 ----------------------------------- //
	AdjustmentItem* adjustmentItem_;

	// --- eyePos/view/Projection ------------------- //
	Vector3 eyePos_;
	Matrix4x4 viewMat_;
	Matrix4x4 projectionMat_;


	std::unique_ptr<FollowCamera> followCamera_;

	std::unique_ptr<Skydome> skydome_;


	std::unique_ptr<TitlePlayer> player_;

	std::unique_ptr<Panel> panel_;
	bool isNextScene_ = false;
	float timeStep_ = 1.5f;

	std::unique_ptr<AudioPlayer> bgm_;
};

