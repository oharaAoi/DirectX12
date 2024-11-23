#include "BossMowDownAttack.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Math/Easing.h"

BossMowDownAttack::~BossMowDownAttack() {
}

void BossMowDownAttack::Init() {
	work_.waitoTime = 2.0f;
	work_.attackTime = 1.0f;
	work_.attackAfterTime = 1.0f;
	work_.offset = { 0.0f, 12.0f, 0.0f };;

	work_.isAttack = false;
	work_.isAttackFinsh_ = false;

	isStop_ = false;

	moveTime_ = 0.0f;

	attackFunction_ = std::bind(&BossMowDownAttack::Aim, this);

	// -------------------------------------------------
	// ↓ 調整項目系
	// -------------------------------------------------
	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	adjust->AddItem(groupName_, "aimPos_left", aimPos_);
	adjust->AddItem(groupName_, "aimPos_right", aimPos_);
	adjust->AddItem(groupName_, "targetPos_left",targetPos_);
	adjust->AddItem(groupName_, "targetPos_right",targetPos_);

	adjust->AddItem(groupName_, "aimTime",work_.attackTime);
	adjust->AddItem(groupName_, "swingTime",work_.attackTime);
	adjust->AddItem(groupName_, "returnTime",work_.attackTime);

	adjust->AddItem(groupName_, "aimEasing", (uint32_t)work_.easeFunction);
	adjust->AddItem(groupName_, "swingEasing", (uint32_t)work_.easeFunction);
	adjust->AddItem(groupName_, "returnEasing", (uint32_t)work_.easeFunction);
}

void BossMowDownAttack::Attack() {
	if (isStop_) {
		return;
	}

	if (attackFunction_) {
		attackFunction_();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　狙う
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossMowDownAttack::Aim() {
	moveTime_ += GameTimer::DeltaTime();
	float t = moveTime_ / work_.attackTime;

	Vector3 movePos = Vector3::Lerp(pBossHand_->beforeAttackPos_, 
									aimPos_,
									CallEasingFunc(work_.easeFunction, t));
	Quaternion moveRotate = Quaternion::Slerp(pBossHand_->localRotate_, attackRotate_, t);

	pBossHand_->transform_->SetTranslaion(movePos);
	pBossHand_->transform_->SetQuaternion(moveRotate);

	if (moveTime_ > work_.attackTime) {
		attackFunction_ = std::bind(&BossMowDownAttack::MowDown, this);
		pBossHand_->beforeAttackPos_ = pBossHand_->transform_->GetTranslation();
		moveTime_ = 0.0f;

		AdjustmentItem* adjust = AdjustmentItem::GetInstance();
		work_.attackTime = adjust->GetValue<float>(groupName_, "swingTime");
		work_.easeFunction = (int)adjust->GetValue<uint32_t>(groupName_, "swingEasing");
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　薙ぎ払う
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossMowDownAttack::MowDown() {
	moveTime_ += GameTimer::DeltaTime();
	float t = moveTime_ / work_.attackTime;

	Vector3 movePos = Vector3::Lerp(pBossHand_->beforeAttackPos_,
									targetPos_,
									CallEasingFunc(work_.easeFunction, t));

	pBossHand_->transform_->SetTranslaion(movePos);

	if (moveTime_ > work_.attackTime) {
		attackFunction_ = std::bind(&BossMowDownAttack::Return, this);
		pBossHand_->beforeAttackPos_ = pBossHand_->transform_->GetTranslation();
		moveTime_ = 0.0f;

		AdjustmentItem* adjust = AdjustmentItem::GetInstance();
		work_.attackTime = adjust->GetValue<float>(groupName_, "returnTime");
		work_.easeFunction = (int)adjust->GetValue<uint32_t>(groupName_, "returnEasing");
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　戻す
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossMowDownAttack::Return() {
	moveTime_ += GameTimer::DeltaTime();
	float t = moveTime_ / work_.attackTime;

	Vector3 movePos = Vector3::Lerp(pBossHand_->beforeAttackPos_,
									pBossHand_->initPos_,
									CallEasingFunc(work_.easeFunction, t));
	Quaternion moveRotate = Quaternion::Slerp(attackRotate_, pBossHand_->localRotate_, t);

	pBossHand_->transform_->SetTranslaion(movePos);
	pBossHand_->transform_->SetQuaternion(moveRotate);

	if (moveTime_ > work_.attackTime) {
		pBossHand_->isAttackMove_ = false;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　調整項目の適応
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossMowDownAttack::AdaptAdjustment(HandType handType) {
	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	if (handType == HandType::LEFT_HAND) {
		aimPos_ = adjust->GetValue<Vector3>(groupName_, "aimPos_left");
		targetPos_ = adjust->GetValue<Vector3>(groupName_, "targetPos_left");
		attackRotate_ = Quaternion::AngleAxis(-90.0f * toRadian, Vector3::FORWARD());
	} else {
		aimPos_ = adjust->GetValue<Vector3>(groupName_, "aimPos_right");
		targetPos_ = adjust->GetValue<Vector3>(groupName_, "targetPos_right");
		attackRotate_ = Quaternion::AngleAxis(90.0f * toRadian, Vector3::FORWARD()) * Quaternion::AngleAxis(180.0f * toRadian, Vector3::UP());
	}

	work_.attackTime = adjust->GetValue<float>(groupName_, "aimTime");
	work_.easeFunction = (int)adjust->GetValue<uint32_t>(groupName_, "aimEasing");
}

#ifdef _DEBUG
void BossMowDownAttack::Debug_Gui() {
	ImGui::Begin("MowDownAttack");
	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	if (ImGui::TreeNode("Aim")) {
		ImGui::DragFloat("attackTime", &work_.attackTime, 0.1f);
		ShowEasingDebug(work_.easeFunction);
		if (ImGui::Button("Save")) {
			adjust->SetValue(groupName_, "aimTime", work_.attackTime);
			adjust->SetValue(groupName_, "aimEasing", (uint32_t)work_.easeFunction);
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Swing")) {
		ImGui::DragFloat("attackTime", &work_.attackTime, 0.1f);
		ShowEasingDebug(work_.easeFunction);
		if (ImGui::Button("Save")) {
			adjust->SetValue(groupName_, "swingTime", work_.attackTime);
			adjust->SetValue(groupName_, "swingEasing", (uint32_t)work_.easeFunction);
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Return")) {
		ImGui::DragFloat("attackTime", &work_.attackTime, 0.1f);
		ShowEasingDebug(work_.easeFunction);
		if (ImGui::Button("Save")) {
			adjust->SetValue(groupName_, "returnTime", work_.attackTime);
			adjust->SetValue(groupName_, "returnEasing", (uint32_t)work_.easeFunction);
		}
		ImGui::TreePop();
	}

	ImGui::End();
}
#endif