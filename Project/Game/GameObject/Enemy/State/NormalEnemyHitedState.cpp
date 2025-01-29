#include "NormalEnemyHitedState.h"
#include "Game/GameObject/Enemy/NormalEnemy.h"

NormalEnemyHitedState::~NormalEnemyHitedState() {
}

void NormalEnemyHitedState::Init() {
	stateName_ = "hited";

	pEnemy_->GetAnimetor()->TransitionAnimation("hited", 0.1f);
}

void NormalEnemyHitedState::Update() {
	if (pEnemy_->GetAnimetor()->GetAnimationClip()->GetIsAnimationFinish()) {
		pEnemy_->SetBehaviorRequest(EnemyBehavior::DEFAULT);
	}
}

