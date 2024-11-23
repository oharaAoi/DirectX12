#pragma once
#include "Game/Action/IAttack.h"
#include "Game/GameObject/Boss/BaseBossHand.h"

class BossGooAttack : public IAttack {
public:

	BossGooAttack(BaseBossHand* pBossHand) : pBossHand_(pBossHand) {};
	~BossGooAttack();

	void Init() override;

	void Attack() override;

	/// <summary>
	/// 狙う
	/// </summary>
	void Aim();

	/// <summary>
	/// 振り下ろす
	/// </summary>
	void Slap();

	/// <summary>
	/// 戻す
	/// </summary>
	void Return();

#ifdef _DEBUG
	void Debug_Gui() override {};
#endif

private:

	BaseBossHand* pBossHand_;

	BaseBossHand::AttackWork work_;
};

