#pragma once
#include "BaseObjectState.h"

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

};

