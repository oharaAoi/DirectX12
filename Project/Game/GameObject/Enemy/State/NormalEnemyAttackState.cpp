#include "NormalEnemyAttackState.h"
#include "Game/GameObject/Enemy/NormalEnemy.h"

NormalEnemyAttackState::~NormalEnemyAttackState() {
}

void NormalEnemyAttackState::Init() {
	stateName_ = "attack";
	pEnemy_->GetAnimetor()->TransitionAnimation("attack");

	work_.time = 0.0f;
	work_.timeLimit = 1.0f;

	velocity_ = (pEnemy_->GetPlayerPos() - pEnemy_->GetTransform()->GetTranslation()).Normalize();
}

void NormalEnemyAttackState::Update() {
	work_.time += GameTimer::DeltaTime();
	
	Vector3 pos = pEnemy_->GetTransform()->GetTranslation();
	pos += velocity_ * GameTimer::DeltaTime();
	pEnemy_->GetTransform()->SetTranslaion(pos);

	if (work_.time >= work_.timeLimit) {
		pEnemy_->SetBehaviorRequest(EnemyBehavior::DEFAULT);
	}
}

#ifdef _DEBUG
void NormalEnemyAttackState::Debug_Gui() {
}
#endif // _DEBUG