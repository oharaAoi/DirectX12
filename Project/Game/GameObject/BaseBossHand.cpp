#include "BaseBossHand.h"

BaseBossHand::BaseBossHand() {
}

BaseBossHand::~BaseBossHand() {
}

void BaseBossHand::Finalize() {
}

void BaseBossHand::Init() {
	// -------------------------------------------------
	// ↓ Colliderに関する設定
	// -------------------------------------------------
	meshCollider_->SetCollisionEnter([this](MeshCollider& other) {OnCollisionEnter(other); });
	meshCollider_->SetCollisionStay([this](MeshCollider& other) {OnCollisionStay(other); });
	meshCollider_->SetCollisionExit([this](MeshCollider& other) {OnCollisionExit(other); });

	// -------------------------------------------------
	// ↓ メンバ変数の初期化
	// -------------------------------------------------
	isGroundSlap_ = false;


}

void BaseBossHand::Update() {
#ifdef _DEBUG
	if (isClicked_) {
		SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	} else {
		SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	}
#endif

	/*if (isAttackMove_) {
		AttackMove(transform_.get());
	}*/

	if (animetor_ != nullptr) {
		animetor_->UpdateScript(animationTime_, animationTransitionTime_);
	}

	/*if (animetor_ != nullptr) {
		if (!animetor_->GetIsAnimationChange()) {
			animationTime_ += GameTimer::DeltaTime();
		}
		animetor_->UpdateScript(animationTime_, animationTransitionTime_);
		animationTime_ = std::fmod(animationTime_, animetor_->GetAnimationDuration());
	}*/

	BaseGameObject::Update();
}

void BaseBossHand::Draw() const {
	Engine::SetPipeline(PipelineType::NormalPipeline);
	BaseGameObject::Draw();

#ifdef _DEBUG
	Engine::SetPipeline(PipelineType::PrimitivePipeline);
	meshCollider_->Draw();
#endif
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
// ↓　攻撃の準備
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseBossHand::PrepareAttack(const AttackType& type) {
	beforeAttackPos_ = transform_->GetTranslation();
	attackType_ = type;
	isAttackMove_ = true;
	attackWork_.isAttack = false;
	isGroundSlap_ = false;
	moveTime_ = 0.0f;

	easingIndex_ = (int)EasingType::Out::Quart;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　攻撃の分岐
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseBossHand::Attack() {
	const auto& it = functionMap_.find(attackType_);
	if (it == functionMap_.end()) {
		assert("not find RootSignature");
	}

	return (this->*(it->second))(); // メンバー関数ポインタを使って関数を呼び出す
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　gooの攻撃
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseBossHand::GooSlap() {
	attackWork_.offset = { 0.0f, 10.0f, 0.0f };
	Vector3 worldPos = transform_->GetTranslation();

	// 攻撃を溜める時間
	if (!attackWork_.isAttack) {
		if (animationTime_ > 0.7f) {
			animationTime_ += GameTimer::DeltaTime();
		}

		moveTime_ += GameTimer::DeltaTime();
		
		float t = moveTime_ / attackWork_.waitoTime;
		// 手を現在の位置からoffestの位置まで移動させる
		Vector3 targetPos = playerPos_ + attackWork_.offset;
		targetPos.z -= bodyPos_.z;
		Vector3 movePos = Vector3::Lerp(beforeAttackPos_, targetPos, CallEasingFunc(easingIndex_, t));
		transform_->SetTranslaion(movePos);

		// 時間を過ぎたら攻撃を行う
		if (moveTime_ > attackWork_.waitoTime) {
			attackWork_.isAttack = true;
			isGroundSlap_ = false;
			moveTime_ = 0.0f;
			easingIndex_ = (int)EasingType::In::Elastic;
		}

	} else if(!isGroundSlap_) { // 攻撃をする時間
		attackVeclocity_ = { 0.0f, -1.0f, 0.0f };
		// 時間ないだったら時間を増やす
		if (moveTime_ < attackWork_.attackTime) {
			moveTime_ += GameTimer::DeltaTime();
		}
		// tで溜め時間を作る
		float t = moveTime_ / attackWork_.attackTime;
		float moveSpeed = std::lerp(0.0f, attackSpeed_, Ease::In::Elastic(t));

		Vector3 movePos = transform_->GetTranslation();
		movePos += (attackVeclocity_ * moveSpeed) * GameTimer::DeltaTime();
		transform_->SetTranslaion(movePos);


	} else if (isGroundSlap_) {
		moveTime_ += GameTimer::DeltaTime();
		if (moveTime_ < attackWork_.attackAfterTime) {
			float t = moveTime_ / attackWork_.attackAfterTime;
			// 手を現在の位置からoffestの位置まで移動させる
			Vector3 movePos = Vector3::Lerp(beforeAttackPos_, initPos_, Ease::In::Sine(t));
			transform_->SetTranslaion(movePos);
		} else {
			isAttackMove_ = false;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　衝突判定
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseBossHand::OnCollisionEnter([[maybe_unused]] MeshCollider& other) {
	if (other.GetTag() == "field") {
		isGroundSlap_ = true;
		moveTime_ = 0.0f;
		beforeAttackPos_ = transform_->GetTranslation();
	}
}

void BaseBossHand::OnCollisionStay([[maybe_unused]] MeshCollider& other) {
}

void BaseBossHand::OnCollisionExit([[maybe_unused]] MeshCollider& other) {
}
