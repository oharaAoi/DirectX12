#include "NormalEnemyDefaultState.h"
#include "Game/GameObject/Enemy/NormalEnemy.h"

NormalEnemyDefaultState::~NormalEnemyDefaultState() {
}

void NormalEnemyDefaultState::Init() {
	stateName_ = "default";

	pEnemy_->GetAnimetor()->TransitionAnimation("default");
}

void NormalEnemyDefaultState::Update() {
}

#ifdef _DEBUG
void NormalEnemyDefaultState::Debug_Gui() {
}
#endif // _DEBUG