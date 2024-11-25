#include "BossCoreHideState.h"
#include "Game/GameObject/Boss/Boss.h"

BossCoreHideState::~BossCoreHideState() {
}

void BossCoreHideState::Init() {
	stateName_ = "hideState";

	defaultPosition_ = { 0.0f,7.0f,-2.0f };
	middlePosition_ = { 0.0f,13.0f,-9.0f };
	endPosition_ = { 0.0f,7.0f, -18.0f };
}

void BossCoreHideState::Update() {
	if (0.0f < moveTime_) {
		moveTime_ -= GameTimer::DeltaTime();
	}
	else if (moveTime_ <= 0.0f) {
		pCore_->SetBehaviorRequest(CoreState::Default);
		pCore_->SetFalling(false);
	}

	float t = (1 - moveTime_);
	Vector3 pos = t * t * defaultPosition_ + 2.0f * t * moveTime_ * middlePosition_ + moveTime_ * moveTime_ * endPosition_;
	pCore_->GetTransform()->SetTranslaion(pos);
}

#ifdef _DEBUG
void BossCoreHideState::Debug_Gui() {
}
#endif // _DEBUG