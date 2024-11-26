#include "BossCoreAppearState.h"
#include "Game/GameObject/Boss/Boss.h"

BossCoreAppearState::~BossCoreAppearState() {
}

void BossCoreAppearState::Init() {
	stateName_ = "appearState";

	defaultPosition_ = { 0.0f,7.0f,-8.0f };
	middlePosition_ = { 0.0f,13.0f,-12.0f };
	endPosition_ = { 0.0f,7.0f, -18.0f };
}

void BossCoreAppearState::Update() {
	if (pCore_->GetIsAppearReset()) {
		appearTime_ = 3.0f;
		pCore_->SetIsAppearReset(false);
	}

	if (1.0f > moveTime_) {
		moveTime_ += GameTimer::DeltaTime();
	}
	else if (moveTime_ >= 1.0f) {
		appearTime_ -= GameTimer::DeltaTime();
		if (appearTime_ <= 0.0f) {
			pCore_->ChangeHide();
		}
	}

	float t = (1 - moveTime_);
	Vector3 pos = t * t * defaultPosition_ + 2.0f * t * moveTime_ * middlePosition_ + moveTime_ * moveTime_ * endPosition_;
	pCore_->GetTransform()->SetTranslaion(pos);
}

#ifdef _DEBUG
void BossCoreAppearState::Debug_Gui() {
}
#endif // _DEBUG