#include "PlayerDefaultState.h"
#include "Game/GameObject/Player/Player.h"

PlayerDefaultState::~PlayerDefaultState() {
}

void PlayerDefaultState::Init() {
	stateName_ = "playerDefaultState";
}

void PlayerDefaultState::Update() {
}

#ifdef _DEBUG
void PlayerDefaultState::Debug_Gui() {
	ImGui::Text(stateName_.c_str());

	
}
#endif // _DEBUG