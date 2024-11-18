#include "BossRootState.h"
#include "Game/GameObject/Boss.h"

BossRootState::~BossRootState() {
}

void BossRootState::Init() {
	stateName_ = "rootState";

	pBoss_->GetBossRightHand()->ChangeRootMove(pBoss_->GetBossRightHand());
}

void BossRootState::Update() {
	pBoss_->GetBossRightHand()->AnimationTimeIncrement(true);
}
