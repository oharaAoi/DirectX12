#pragma once
#include "Game/GameObject/State/BaseObjectState.h"
#include "Game/GameObject/BossRightHand.h"
#include "Game/GameObject/BossLeftHand.h"

class Boss;

/// <summary>
/// 攻撃をする状態
/// </summary>
class BossAttackState
	: public BaseObjectState {
public:

	BossAttackState(Boss* pBoss) : pBoss_(pBoss) { Init(); };
	~BossAttackState();

	void Init()override;
	void Update() override;

private:

	Boss* pBoss_ = nullptr;

	BossRightHand* rightHand_;
	BossLeftHand* leftHand_;

};

