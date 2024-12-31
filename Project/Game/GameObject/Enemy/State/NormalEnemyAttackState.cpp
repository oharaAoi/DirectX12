#include "NormalEnemyAttackState.h"
#include "Game/GameObject/Enemy/NormalEnemy.h"

NormalEnemyAttackState::~NormalEnemyAttackState() {
}

void NormalEnemyAttackState::Init() {
	stateName_ = "attack";

	pEnemy_->GetAnimetor()->TransitionAnimation("attack");
}

void NormalEnemyAttackState::Update() {
}

#ifdef _DEBUG
void NormalEnemyAttackState::Debug_Gui() {
}
#endif // _DEBUG