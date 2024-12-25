#include "PlayerAvoidanceState.h"
#include "Game/GameObject/Player/Player.h"

PlayerAvoidanceState::~PlayerAvoidanceState() {
}

void PlayerAvoidanceState::Init() {
	stateName_ = "playerAvoidanceState";
}

void PlayerAvoidanceState::Update() {
}

#ifdef _DEBUG
void PlayerAvoidanceState::Debug_Gui() {
}
#endif // _DEBUG