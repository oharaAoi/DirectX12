#pragma once
#include "Game/Scene/BaseScene.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Game/Camera/Camera.h"
#include "Game/Camera/DebugCamera.h"
#include "Game/WorldObject/Skydome.h"
#include "Game/WorldObject/BackGround.h"
#include "Game/GameObject/Boss/Boss.h"

class GameOverScene
	: public BaseScene {
public:

	GameOverScene();
	~GameOverScene() override;

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

#ifdef _DEBUG
	void Debug_Gui();
#endif

	// --- 調整項目 ----------------------------------- //
	AdjustmentItem* adjustmentItem_;

	// --- eyePos/view/Projection ------------------- //
	Vector3 eyePos_;
	Matrix4x4 viewMat_;
	Matrix4x4 projectionMat_;

	// --- Camera ---------------------------------- //
	std::unique_ptr<DebugCamera> debugCamera_;
	bool isDebugCamera_;

	std::unique_ptr<Camera> camera_;

	// --- WorldObject ----------------------------- //
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<BackGround> backGround_;

	// --- GameObject ------------------------------ //

	std::unique_ptr<Boss> boss_;

	// --- Light ----------------------------------- //
	SpotLight* spotLight_;
};
