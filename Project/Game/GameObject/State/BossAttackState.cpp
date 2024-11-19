#include "BossAttackState.h"
#include "Game/GameObject/Boss.h"

BossAttackState::~BossAttackState() {
}

void BossAttackState::Init() {
	stateName_ = "AttackState";

	rightHand_ = pBoss_->GetBossRightHand();
	leftHand_ = pBoss_->GetBossLeftHand();

}

void BossAttackState::Update() {
	if (rightHand_->GetIsAttackMove()) {
		rightHand_->Attack();
	}

	if (leftHand_->GetIsAttackMove()) {
		leftHand_->Attack();
	}

	// どちらの手も攻撃していなかったら
	if (!leftHand_->GetIsAttackMove() && !rightHand_->GetIsAttackMove()) {
		pBoss_->SetBehaviorRequest(Behavior::ROOT);
	}
}
