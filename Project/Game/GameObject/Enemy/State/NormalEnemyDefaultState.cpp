#include "NormalEnemyDefaultState.h"
#include "Game/GameObject/Enemy/NormalEnemy.h"

NormalEnemyDefaultState::~NormalEnemyDefaultState() {
}

void NormalEnemyDefaultState::Init() {
	stateName_ = "default";

	pEnemy_->GetAnimetor()->TransitionAnimation("default", 0.5f);
}

void NormalEnemyDefaultState::Update() {
	if ((pEnemy_->GetTransform()->GetTranslation() - pEnemy_->GetPlayerPos()).Length() < 10.0f) {
		pEnemy_->SetBehaviorRequest(EnemyBehavior::MOVE);
	}
}

#ifdef _DEBUG
void NormalEnemyDefaultState::Debug_Gui() {
}
#endif // _DEBUG