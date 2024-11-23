#include "BossCoreDefaultState.h"
#include "Game/GameObject/Boss/Boss.h"

BossCoreDefaultState::~BossCoreDefaultState() {
}

void BossCoreDefaultState::Init() {
	stateName_ = "defaultState";
}

void BossCoreDefaultState::Update() {

}

#ifdef _DEBUG
void BossCoreDefaultState::Debug_Gui() {
}
#endif // _DEBUG