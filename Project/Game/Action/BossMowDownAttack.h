#pragma once
#include <functional>
#include <string>
#include "Game/Action/IAttack.h"
#include "Game/GameObject/Boss/BaseBossHand.h"

class BossMowDownAttack : public IAttack {
public:

	BossMowDownAttack(BaseBossHand* pBossHand) : pBossHand_(pBossHand) {};
	~BossMowDownAttack();

	void Init() override;

	void Attack() override;

	/// <summary>
	/// 狙う
	/// </summary>
	void Aim();

	/// <summary>
	/// 振り下ろす
	/// </summary>
	void MowDown();

	/// <summary>
	/// 戻す
	/// </summary>
	void Return();

	/// <summary>
	/// 調整項目の適応
	/// </summary>
	/// <param name="handType"></param>
	void AdaptAdjustment(HandType handType);

#ifdef _DEBUG
	void Debug_Gui() override;
#endif

private:

	const std::string groupName_ = "mowDownAttack";

	BaseBossHand* pBossHand_;

	BaseBossHand::AttackWork work_;

	std::function<void()> attackFunction_;

	Vector3 aimPos_;
	Vector3 targetPos_;

	Quaternion attackRotate_;

	bool isStop_;

};

