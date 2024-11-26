#include "BossParAttack.h"

BossParAttack::~BossParAttack() {
}

void BossParAttack::Init() {
	work_.waitoTime = 2.0f;
	work_.attackTime = 1.0f;
	work_.attackAfterTime = 1.0f;
	work_.offset = { 0.0f, 12.0f, 0.0f };;

	work_.isAttack = false;
	work_.isAttackFinsh_ = false;

	moveTime_ = 0.0f;
}

void BossParAttack::Attack() {
	work_.offset = { 0.0f, 12.0f, 0.0f };
	Vector3 worldPos = pBossHand_->transform_->GetTranslation();

	if (!work_.isAttack) {
		Aim();	// 狙う

	} else if (!pBossHand_->isGroundSlap_) {
		Slap();	// 攻撃

	} else if (pBossHand_->isGroundSlap_) {
		Return();	// 戻す
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　狙う
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossParAttack::Aim() {
	if (pBossHand_->animationTime_ > 0.7f) {
		pBossHand_->animationTime_ += GameTimer::DeltaTime();
	}

	moveTime_ += GameTimer::DeltaTime();

	float t = moveTime_ / work_.waitoTime;

	// 手を現在の位置からoffestの位置まで移動させる
	Vector3 targetPos = pBossHand_->playerPos_ + work_.offset;
	targetPos.z -= pBossHand_->bodyPos_.z;
	Vector3 movePos = Vector3::Lerp(pBossHand_->beforeAttackPos_,
									targetPos,
									CallEasingFunc(pBossHand_->easingIndex_, t));
	pBossHand_->transform_->SetTranslaion(movePos);

	// 時間を過ぎたら攻撃を行う
	if (moveTime_ > work_.waitoTime) {
		work_.isAttack = true;
		moveTime_ = 0.0f;

		pBossHand_->isGroundSlap_ = false;
		pBossHand_->easingIndex_ = (int)EasingType::In::Elastic;
	}

	pBossHand_->meshCollider_->SetSubTag("wait_hand");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　振り下ろす
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossParAttack::Slap() {
	pBossHand_->attackVeclocity_ = { 0.0f, -1.0f, 0.0f };

	// 時間ないだったら時間を増やす
	if (moveTime_ < work_.attackTime) {
		moveTime_ += GameTimer::DeltaTime();
	}
	// tで溜め時間を作る
	float t = moveTime_ / work_.attackTime;
	float moveSpeed = std::lerp(0.0f, pBossHand_->attackSpeed_, Ease::In::Elastic(t));

	Vector3 movePos = pBossHand_->transform_->GetTranslation();

	movePos += (pBossHand_->attackVeclocity_ * moveSpeed) * GameTimer::DeltaTime();
	pBossHand_->transform_->SetTranslaion(movePos);

	pBossHand_->meshCollider_->SetSubTag("slap_attack");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　戻す
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossParAttack::Return() {
	moveTime_ += GameTimer::DeltaTime();

	if (moveTime_ < work_.attackAfterTime) {
		float t = moveTime_ / work_.attackAfterTime;

		// 手を現在の位置からoffestの位置まで移動させる
		Vector3 movePos = Vector3::Lerp(pBossHand_->beforeAttackPos_,
										pBossHand_->initPos_,
										Ease::In::Sine(t));
		pBossHand_->transform_->SetTranslaion(movePos);
	} else {
		pBossHand_->isAttackMove_ = false;
		pBossHand_->isGroundSlap_ = false;
	}

	pBossHand_->meshCollider_->SetSubTag("wait_hand");
}
