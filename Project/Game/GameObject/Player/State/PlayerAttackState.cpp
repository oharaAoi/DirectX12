#include "PlayerAttackState.h"
#include "Game/GameObject/Player/Player.h"
#include "Engine/Utilities/AdjustmentItem.h"

PlayerAttackState::~PlayerAttackState() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerAttackState::Init() {
	stateName_ = "playerAttackState";

	work_.time = 0.0f;
	work_.timeLimit = 1.0f;

	information_.FromJson(AdjustmentItem::GetData(stateName_, stateName_));
	pPlayer_->GetAnimetor()->TransitionAnimation(information_.animationName);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerAttackState::Update() {
	if (pPlayer_->GetIsStateDebug()) {
		return ;
	}

	work_.time += GameTimer::DeltaTime();

	if (work_.time >= work_.timeLimit) {
		pPlayer_->SetBehaviorRequest(Behavior::DEFAULT);
		pPlayer_->SetIsAttack(false);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void PlayerAttackState::Debug_Gui() {
	ImGui::Text(stateName_.c_str());
	ImGui::SliderFloat("work_.time", &work_.time, 0.0f, work_.timeLimit);

	information_.animationName = pPlayer_->GetAnimetor()->SelectAnimationName();
	ImGui::Text(information_.animationName.c_str());
	if (ImGui::Button("Save")) {
		AdjustmentItem::Save(stateName_, information_.ToJson(stateName_));
	}
}
#endif // _DEBUG