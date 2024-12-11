#include "PlayerAttackState.h"
#include "Game/GameObject/Player/Player.h"

PlayerAttackState::~PlayerAttackState() {
}

void PlayerAttackState::Init() {
	stateName_ = "playerAttackState";

	work_.time = 0.0f;
	work_.timeLimit = 1.0f;
}

void PlayerAttackState::Update() {
	work_.time += GameTimer::DeltaTime();

	if (work_.time >= work_.timeLimit) {
		pPlayer_->SetBehaviorRequest(Behavior::DEFAULT);
		pPlayer_->SetIsAttack(false);
	}
}

#ifdef _DEBUG
void PlayerAttackState::Debug_Gui() {
	ImGui::Text(stateName_.c_str());
	ImGui::SliderFloat("work_.time", &work_.time, 0.0f, work_.timeLimit);
}
#endif // _DEBUG