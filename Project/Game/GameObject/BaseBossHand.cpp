#include "BaseBossHand.h"

BaseBossHand::BaseBossHand() {
}

BaseBossHand::~BaseBossHand() {
}

void BaseBossHand::AnimeTimeIncrement(bool isLoop, float limitTime) {
	animationTime_ += GameTimer::DeltaTime();

	if (isLoop) {
		animationTime_ = std::fmod(animationTime_, limitTime);
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　攻撃の動きをする
//////////////////////////////////////////////////////////////////////////////////////////////////
void BaseBossHand::AttackMove(WorldTransform* worldTransform) {
	moveTime_ += GameTimer::DeltaTime();
	if (pAttackEditer_->GetHandMoves().size() - 1 > moveIndex_) {
		/*Vector3 scale = transform_->GetScale();
		Quaternion rotate = transform_->GetQuaternion();
		Vector3 translate = transform_->GetTranslation();*/

		float t = moveTime_ / pAttackEditer_->GetHandMoves()[moveIndex_].moveTimeLimit;
		int easeType = pAttackEditer_->GetHandMoves()[moveIndex_].easeType;

		Vector3 scale = Vector3::Lerp(pAttackEditer_->GetHandMoves()[moveIndex_].scale, pAttackEditer_->GetHandMoves()[moveIndex_ + 1].scale, CallEasingFunc(easeType, t));
		Vector3 translate = Vector3::Lerp(pAttackEditer_->GetHandMoves()[moveIndex_].translate, pAttackEditer_->GetHandMoves()[moveIndex_ + 1].translate, CallEasingFunc(easeType, t));

		worldTransform->SetScale(scale);
		worldTransform->SetTranslaion(translate);
	}

	if (moveTime_ > 1.0f) {
		if (pAttackEditer_->GetHandMoves().size() - 1 == moveIndex_) {
			isAttackMove_ = false;
		} else {
			moveIndex_++;
			moveTime_ = 0;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　待機モーションに移動させる
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseBossHand::ChangeRootMove(BaseGameObject* object) {
	object->GetAnimetor()->SetTransitionAnimation(nowAnimatonName_, "stand_by");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　手とマウスカーソルが当たったのかの判定をとる
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseBossHand::CheckMouseCursorCollision(WorldTransform* worldTransform, const Matrix4x4& vpvpMat) {
	Vector2 mousePos = Input::GetMousePosition();

	// objectのscreen座標を求める
	Vector3 objectScreen = Transform(Vector3::ZERO(), worldTransform->GetWorldMatrix() * vpvpMat);
	objectScreenPos_ = Vector2(objectScreen.x, objectScreen.y);

	bool isNear = false;
	// 長さを取って距離が近かったら
	if ((mousePos - objectScreenPos_).Length() < 100.0f) {
		isNear = true;
	}

	// カーソルと手が近い時はtrue
	if (isNear) {
		if (!isClicked_) {
			if (Input::IsPressMouse(0)) {
				isClicked_ = true;
			}
		} else {
			if (Input::IsPressMouse(0)) {
				isClicked_ = false;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　gooの攻撃
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseBossHand::GooSlap(WorldTransform* worldTransform, const Vector3& playerPos) {
	attackWork_.offset = { 0.0f, 3.0f, 0.0f };
	Vector3 worldPos = worldTransform->GetTranslation();
	moveTime_ += GameTimer::DeltaTime();

	// 攻撃を溜める時間
	if (!attackWork_.isAttack) {
		if (animationTime_ > 0.7f) {
			animationTime_ += GameTimer::DeltaTime();
		}
		
		float t = moveTime_ / attackWork_.waitoTime;
		// 手を現在の位置からoffestの位置まで移動させる
		Vector3 movePos = Vector3::Lerp(beforeAttackPos_, playerPos + attackWork_.offset, CallEasingFunc(easingIndex_, t));
		worldTransform->SetTranslaion(movePos);

		// 時間を過ぎたら攻撃を行う
		if (moveTime_ > attackWork_.waitoTime) {
			attackWork_.isAttack = true;
			moveTime_ = 0.0f;
			easingIndex_ = (int)EasingType::In::Elastic;
		}

	} else { // 攻撃をする時間
		float t = moveTime_ / attackWork_.attackTime;
		// 手を現在の位置からoffestの位置まで移動させる
		Vector3 movePos = Vector3::Lerp(beforeAttackPos_, playerPos, CallEasingFunc(easingIndex_, t));
		worldTransform->SetTranslaion(movePos);

		// 時間を過ぎたら攻撃を終了する
		if (moveTime_ > attackWork_.attackTime) {
			attackWork_.isAttackFinsh_ = true;
		}
	}
}
