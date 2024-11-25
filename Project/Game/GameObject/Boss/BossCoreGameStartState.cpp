#include "BossCoreGameStartState.h"
#include "Game/GameObject/Boss/Boss.h"
#include "Engine/Utilities/AdjustmentItem.h"

BossCoreGameStartState::~BossCoreGameStartState() {
}

void BossCoreGameStartState::Init() {
	stateName_ = "gameStartState";

	defaultPosition_ = { 0.0f,7.0f,-2.0f };
	middlePosition_ = { 0.0f,13.0f,-9.0f };
	endPosition_ = { 0.0f,7.0f, -18.0f };
}

void BossCoreGameStartState::Update() {
	if (pCore_->GetIsFirstHit()) {

	}
}

#ifdef _DEBUG
void BossCoreGameStartState::Debug_Gui() {

}
#endif // _DEBUG