#include "PlayerDamageState.h"
#include "Game/GameObject/Player/Player.h"
#include "Engine/Utilities/AdjustmentItem.h"

PlayerDamageState::~PlayerDamageState() {
}

void PlayerDamageState::Init() {
	stateName_ = "playerDamageState";

	information_.FromJson(AdjustmentItem::GetData(stateName_, stateName_));
	pPlayer_->GetAnimetor()->TransitionAnimation(information_.animationName);
}

void PlayerDamageState::Update() {
}

#ifdef _DEBUG
void PlayerDamageState::Debug_Gui() {
	ImGui::Text(stateName_.c_str());
	information_.animationName = pPlayer_->GetAnimetor()->SelectAnimationName();
	ImGui::Text(information_.animationName.c_str());
	if (ImGui::Button("Save")) {
		AdjustmentItem::Save(stateName_, information_.ToJson(stateName_));
	}
}
#endif // _DEBUG