#include "BossGooAttack.h"

BossGooAttack::~BossGooAttack() {
}

void BossGooAttack::Init() {
	work_.waitoTime = 2.0f;
	work_.attackTime = 0.4f;
	work_.attackAfterTime = 1.0f;
	work_.offset = { 0.0f, 12.0f, 0.0f };;

	work_.isAttack = false;	
	work_.isAttackFinsh_;

	moveTime_ = 0.0f;
}

void BossGooAttack::Attack() {
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

void BossGooAttack::Aim() {
	if (pBossHand_->animationTime_ > 0.7f) {
		pBossHand_->animationTime_ += GameTimer::DeltaTime();
	}

	moveTime_ += GameTimer::DeltaTime();

	float t = moveTime_ / work_.waitoTime;

	if (moveTime_ > work_.waitoTime - 0.4f) {
		// 手を現在の位置からoffestの位置まで移動させる
		Vector3 targetPos = playerPos_ + work_.offset;
		targetPos.z -= pBossHand_->bodyPos_.z;
		Vector3 movePos = Vector3::Lerp(pBossHand_->beforeAttackPos_,
										targetPos,
										CallEasingFunc(pBossHand_->easingIndex_, t));
		pBossHand_->transform_->SetTranslaion(movePos);

	} else {
		// 手を現在の位置からoffestの位置まで移動させる
		Vector3 targetPos = pBossHand_->playerPos_ + work_.offset;
		targetPos.z -= pBossHand_->bodyPos_.z;
		Vector3 movePos = Vector3::Lerp(pBossHand_->beforeAttackPos_,
										targetPos,
										CallEasingFunc(pBossHand_->easingIndex_, t));
		pBossHand_->transform_->SetTranslaion(movePos);
		playerPos_ = pBossHand_->playerPos_;
	}

	// 時間を過ぎたら攻撃を行う
	if (moveTime_ > work_.waitoTime) {
		work_.isAttack = true;
		moveTime_ = 0.0f;

		pBossHand_->isGroundSlap_ = false;
		pBossHand_->easingIndex_ = (int)EasingType::In::Elastic;

		swingUpY_ = 0.0f;
	}

	pBossHand_->meshCollider_->SetSubTag("wait_hand");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　振り下ろす
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossGooAttack::Slap() {
	// 時間ないだったら時間を増やす
	if (moveTime_ < work_.attackTime) {
		moveTime_ += GameTimer::DeltaTime();
	}
	// tで溜め時間を作る
	float t = moveTime_ / work_.attackTime;
	float moveSpeed = std::lerp(0.0f, pBossHand_->attackSpeed_, Ease::In::Elastic(t));

	swingUpY_ = std::lerp(2.0f, -2.0f, Ease::Out::Expo(t));
	pBossHand_->attackVeclocity_ = { 0.0f, swingUpY_, 0.0f };

	Vector3 movePos = pBossHand_->transform_->GetTranslation();
	movePos += (pBossHand_->attackVeclocity_ * (moveSpeed * 2.0f)) * GameTimer::DeltaTime();
	pBossHand_->transform_->SetTranslaion(movePos);

	pBossHand_->meshCollider_->SetSubTag("slap_attack");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　戻す
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossGooAttack::Return() {
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
