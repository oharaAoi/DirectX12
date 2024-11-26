#pragma once
#include "Game/Action/IAttack.h"
#include "Game/GameObject/Boss/BaseBossHand.h"

class BossParAttack : public IAttack {
public:

	BossParAttack(BaseBossHand* pBossHand) : pBossHand_(pBossHand) {};
	~BossParAttack();

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

	float swingUpY_;

	Vector3 playerPos_;

};

