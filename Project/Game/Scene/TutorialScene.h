#pragma once
#include <memory>
#include "Game/Scene/BaseScene.h"
#include "Game/Camera/FollowCamera.h"
#include "Game/Camera/DebugCamera.h"
#include "Game/WorldObject/Skydome.h"
#include "Game/WorldObject/Field.h"
#include "Game/GameObject/Player/Player.h"

#include "Game/UI/Panel.h"

class TutorialScene
	: public BaseScene {
public:

	TutorialScene();
	~TutorialScene() override;

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void AutoUpdate();
	void TutorialUpdate();

	/// <summary>
	/// Playerが一定以上進んだら自動で進める
	/// </summary>
	void AutoMove();

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	// --- eyePos/view/Projection ------------------- //
	Vector3 eyePos_;
	Matrix4x4 viewMat_;
	Matrix4x4 projectionMat_;

	// --- Camera ---------------------------------- //
	std::unique_ptr<DebugCamera> debugCamera_;
	bool isDebugCamera_;

	std::unique_ptr<FollowCamera> followCamera_;

	// --- WorldObject ----------------------------- //
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Field> field_;
	std::unique_ptr<BaseGameObject> wall_;

	// --- GameObject ------------------------------ //
	std::unique_ptr<Player> player_;

	// --- UI -------------------------------------- //
	std::unique_ptr<Panel> panel_;

	bool isNextScene_ = false;
};

