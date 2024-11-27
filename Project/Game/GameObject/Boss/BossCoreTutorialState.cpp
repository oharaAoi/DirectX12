#include "BossCoreTutorialState.h"
#include "Game/GameObject/Boss/Boss.h"

BossCoreTutorialState::~BossCoreTutorialState() {
}

void BossCoreTutorialState::Init() {
	stateName_ = "TutorialState";

	pos_ = { 6.0f, 2.0f, 0.0f };
}

void BossCoreTutorialState::Update() {
	pCore_->GetTransform()->SetTranslaion(pos_);
}

#ifdef _DEBUG
void BossCoreTutorialState::Debug_Gui() {
}
#endif // _DEBUG