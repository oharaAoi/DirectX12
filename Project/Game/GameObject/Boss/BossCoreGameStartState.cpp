#include "BossCoreGameStartState.h"
#include "Game/GameObject/Boss/Boss.h"
#include "Engine/Utilities/AdjustmentItem.h"

BossCoreGameStartState::~BossCoreGameStartState() {
}

void BossCoreGameStartState::Init() {
	stateName_ = "gameStartState";

	/*defaultPosition_ = { 0.0f,7.0f,-2.0f };
	middlePosition_ = { 0.0f,13.0f,-9.0f };
	endPosition_ = { 0.0f,7.0f, -18.0f };*/

	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	adjust->AddItem(groupName_, "defaultPosition", defaultPosition_);
	adjust->AddItem(groupName_, "middlePosition", middlePosition_);
	adjust->AddItem(groupName_, "endPosition", endPosition_);

	defaultPosition_ = adjust->GetValue<Vector3>(groupName_, "defaultPosition");
	middlePosition_ = adjust->GetValue<Vector3>(groupName_, "middlePosition");
	endPosition_ = adjust->GetValue<Vector3>(groupName_, "endPosition");

	pCore_->GetTransform()->SetTranslaion(defaultPosition_);

	moveTime_ = 0.0f;

	isMove_ = false;
}

void BossCoreGameStartState::Update() {
	if (pCore_->GetIsFirstHit()) {
		isMove_ = true;
	}

	if (isMove_) {
		moveTime_ += GameTimer::DeltaTime();
		float t = (1 - moveTime_);
		Vector3 pos = t * t * defaultPosition_ + 2.0f * t * moveTime_ * middlePosition_ + moveTime_ * moveTime_ * endPosition_;
		pCore_->GetTransform()->SetTranslaion(pos);

		if (moveTime_ >= 1.0f) {
			isMove_ = false;
			moveTime_ = 0.0f;
			pCore_->SetBehaviorRequest(CoreState::Default);
		}
	}

}

#ifdef _DEBUG
void BossCoreGameStartState::Debug_Gui() {
	ImGui::Begin("BossCoreGameStartState");
	ImGui::DragFloat3("defaultPosition", &defaultPosition_.x, 0.1f);
	ImGui::DragFloat3("middlePosition", &middlePosition_.x, 0.1f);
	ImGui::DragFloat3("endPosition", &endPosition_.x, 0.1f);

	if (ImGui::Button("test")) {
		isMove_ = true;
		moveTime_ = 0.0f;
	}

	ImGui::End();
}
#endif // _DEBUG