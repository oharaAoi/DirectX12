#include "BossAppearState.h"
#include "Game/GameObject/Boss/Boss.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Math/Easing.h"

BossAppearState::~BossAppearState() {
}

void BossAppearState::Init() {
	stateName_ = "AppearState";

	isAppear_ = false;
	isShine_ = false;

	moveTime_ = 0.0f;
	moveTimeLimit_ = 4.0f;

	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	adjust->AddItem(groupName_, "startPos", startPos_);
	adjust->AddItem(groupName_, "endPos", endPos_);
	adjust->AddItem(groupName_, "appearEasing", (uint32_t)appearEasing_);

	AdaptAdjustment();	
}

void BossAppearState::Update() {
	// コアの状態が外に出ている状態だったら更新を行わない
	if (pBoss_->GetBossCore()->GetCoreState() != CoreState::Default) {
		return;
	} else if(!isShine_) {
		isShine_ = true;
		pBoss_->GetBossEye()->SetShine();
	}

	// コアがしまわれたら目を光らせる
	if (!pBoss_->GetBossEye()->GetIsShine()) {
		if (!isAppear_) {
			isAppear_ = true;
		}
	} else {
		return;
	}

	// 登場する用になったら移動させる
	if (isAppear_) {
		moveTime_ += GameTimer::DeltaTime();
		moveTime_ = std::clamp(moveTime_, 0.0f, moveTimeLimit_);

		float t = moveTime_ / moveTimeLimit_;
		movePos_ = Vector3::Lerp(startPos_, endPos_, CallEasingFunc(appearEasing_, t));

		if (moveTime_ >= moveTimeLimit_) {
			isAppear_ = false;
			pBoss_->SetBehaviorRequest(Behavior::ROOT);
			pBoss_->SetIsAppear(true);
		}

		pBoss_->GetBossBody()->GetTransform()->SetTranslaion(movePos_);
	} 
}

void BossAppearState::AdaptAdjustment() {
	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	startPos_ = adjust->GetValue<Vector3>(groupName_, "startPos");
	endPos_ = adjust->GetValue<Vector3>(groupName_, "endPos");
	appearEasing_ = (int)adjust->GetValue<uint32_t>(groupName_, "appearEasing");
}

#ifdef _DEBUG
void BossAppearState::Debug_Gui() {
	ImGui::Begin("BossAppearState");
	ImGui::DragFloat3("startPos", &startPos_.x, 0.1f);
	ImGui::DragFloat3("endPos", &endPos_.x, 0.1f);
	ShowEasingDebug(appearEasing_);
	
	if (ImGui::Button("test")) {
		isAppear_ = true;
		moveTime_ = 0.0f;
	}

	if (ImGui::Button("Save")) {
		AdjustmentItem* adjust = AdjustmentItem::GetInstance();
		adjust->SetValue(groupName_, "startPos", startPos_);
		adjust->SetValue(groupName_, "endPos", endPos_);
		adjust->SetValue(groupName_, "appearEasing", (uint32_t)appearEasing_);
	}

	ImGui::End();
}
#endif // _DEBUG