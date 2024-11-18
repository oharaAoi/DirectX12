#include "BossAttackState.h"
#include "Game/GameObject/Boss.h"

BossAttackState::~BossAttackState() {
}

void BossAttackState::Init() {
	stateName_ = "AttackState";

	rightHand_ = pBoss_->GetBossRightHand();
	leftHand_ = pBoss_->GetBossLeftHand();

	rightHand_->PrepareAttack(rightHand_->GetTransform()->GetTranslation());
}

void BossAttackState::Update() {
	rightHand_->GooSlap(pBoss_->GetPlayerPos());
}
