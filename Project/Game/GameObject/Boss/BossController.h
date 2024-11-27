#pragma once
#include "Game/GameObject/Boss/Boss.h"
#include "Game/GameObject/Boss/BossLeftHand.h"
#include "Game/GameObject/Boss/BossRightHand.h"

class BossController {
public:

	BossController();
	~BossController();

	void Init(Boss* boss);
	void Update(bool goTitle);
	void PostUpdate();
	void Draw() const;

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	Boss* boss_;

	BossLeftHand* leftHand_;
	BossRightHand* rightHand_;

	std::unique_ptr<BaseGameObject> title_;
	std::unique_ptr<BaseGameObject> retry_;

	bool preSelect_;

	float moveTitleZ_;
	float moveRetryZ_;

	float moveSpeed_ = 9.0f;

	float moveTime_;
	float moveTimeLimit_;

	Vector3 rightHandInitPos_;
	Vector3 leftHandInitPos_;

	float titleMoveRotate_;
	float retryMoveRotate_;

	Quaternion titleRotate_;
	Quaternion retryRotate_;

	bool isMove_;
	bool isRightMove_;
};

