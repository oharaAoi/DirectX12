#include "NormalEnemyHitedState.h"
#include "Game/GameObject/Enemy/NormalEnemy.h"

NormalEnemyHitedState::~NormalEnemyHitedState() {
}

void NormalEnemyHitedState::Init() {
	stateName_ = "hited";

	pEnemy_->GetAnimetor()->TransitionAnimation("hited", 0.1f);
}

void NormalEnemyHitedState::Update() {

	pEnemy_->GetTransform()->translate_ += (pEnemy_->GetKnockBackVelocity() * 2.0f) * GameTimer::DeltaTime();

	if (pEnemy_->GetAnimetor()->GetAnimationClip()->GetIsAnimationFinish()) {
		pEnemy_->SetBehaviorRequest(EnemyBehavior::DEFAULT);
	}
}

