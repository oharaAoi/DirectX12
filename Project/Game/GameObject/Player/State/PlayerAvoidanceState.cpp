#include "PlayerAvoidanceState.h"
#include "Game/GameObject/Player/Player.h"
#include "Engine/Utilities/AdjustmentItem.h"

PlayerAvoidanceState::~PlayerAvoidanceState() {
}

void PlayerAvoidanceState::Init() {
	stateName_ = "playerAvoidanceState";

	work_.time = 0.0f;
	work_.timeLimit = 1.8f;

	information_.FromJson(AdjustmentItem::GetData(stateName_, stateName_));
	pPlayer_->GetAnimetor()->TransitionAnimation(information_.animationName);
}

void PlayerAvoidanceState::Update() {

	work_.time += GameTimer::DeltaTime();
	if (work_.time >= work_.timeLimit) {
		pPlayer_->SetBehaviorRequest(Behavior::DEFAULT);
	}
}

#ifdef _DEBUG
void PlayerAvoidanceState::Debug_Gui() {
	ImGui::Text(stateName_.c_str());
	
	information_.animationName = pPlayer_->GetAnimetor()->SelectAnimationName();
	ImGui::Text(information_.animationName.c_str());
	if (ImGui::Button("Save")) {
		AdjustmentItem::Save(stateName_, information_.ToJson(stateName_));
	}
}
#endif // _DEBUG