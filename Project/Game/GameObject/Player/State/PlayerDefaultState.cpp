#include "PlayerDefaultState.h"
#include "Game/GameObject/Player/Player.h"
#include "Engine/Utilities/AdjustmentItem.h"

PlayerDefaultState::~PlayerDefaultState() {
}

void PlayerDefaultState::Init() {
	stateName_ = "playerDefaultState";
	/*AdjustmentItem::Save(stateName_, information_.ToJson(stateName_));*/
	information_.FromJson(AdjustmentItem::GetData(stateName_, stateName_));
}

void PlayerDefaultState::Update() {
}

#ifdef _DEBUG
void PlayerDefaultState::Debug_Gui() {
	ImGui::Text(stateName_.c_str());


}
#endif // _DEBUG