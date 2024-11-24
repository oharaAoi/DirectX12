#include "BaseBossHand.h"
#include "Game/Action/BossGooAttack.h"
#include "Game/Action/BossParAttack.h"
#include "Game/Action/BossMowDownAttack.h"

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

	isAlive_ = true;

	hp_ = kDurability_;

	nowAnimatonName_ = "stand_by";
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
}

#ifdef _DEBUG
void BaseBossHand::Debug_Draw() {
	meshCollider_->Draw();
}
#endif

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
	if (pAttackEditer_->GetHandMoves().size() - 1 > moveIndex_) {
		float t = 0.1f / pAttackEditer_->GetHandMoves()[moveIndex_].moveTimeLimit;
		int easeType = pAttackEditer_->GetHandMoves()[moveIndex_].easeType;

		Vector3 scale = Vector3::Lerp(pAttackEditer_->GetHandMoves()[moveIndex_].scale, pAttackEditer_->GetHandMoves()[moveIndex_ + 1].scale, CallEasingFunc(easeType, t));
		Vector3 translate = Vector3::Lerp(pAttackEditer_->GetHandMoves()[moveIndex_].translate, pAttackEditer_->GetHandMoves()[moveIndex_ + 1].translate, CallEasingFunc(easeType, t));

		Quaternion rotate = Quaternion::Slerp(pAttackEditer_->GetHandMoves()[moveIndex_].rotate,
											  pAttackEditer_->GetHandMoves()[moveIndex_ + 1].rotate,
											  CallEasingFunc(easeType, t));

		worldTransform->SetScale(scale);
		worldTransform->SetTranslaion(translate);
		worldTransform->SetQuaternion(rotate);
	}

	if (0.1f > 1.0f) {
		if (pAttackEditer_->GetHandMoves().size() - 1 == moveIndex_) {
			isAttackMove_ = false;
		} else {
			moveIndex_++;
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
// ↓　攻撃の準備
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseBossHand::PrepareAttack(const AttackType& type) {
	beforeAttackPos_ = transform_->GetTranslation();
	attackType_ = type;
	isAttackMove_ = true;
	attackWork_.isAttack = false;
	isGroundSlap_ = false;

	easingIndex_ = (int)EasingType::Out::Quart;

	animationTransitionTime_ = 1.0f;

	switch (type) {
	case AttackType::GooSlap_Attack:
		//animetor_->SetTransitionAnimation("stand_by", "slap");

		meshCollider_->SetSubTag("slap_attack");
		attackAction_ = std::make_unique<BossGooAttack>(this);
		attackAction_->Init();
		attackType_ = AttackType::GooSlap_Attack;

		break;
	case AttackType::ParSlap_Attack:
		meshCollider_->SetSubTag("slap_attack");
		attackAction_ = std::make_unique<BossParAttack>(this);
		attackAction_->Init();
		attackType_ = AttackType::ParSlap_Attack;
		break;
	case AttackType::Missile_Attack:
		attackType_ = AttackType::Missile_Attack;
		break;

	case AttackType::MowDown_Attack:
		attackAction_ = std::make_unique<BossMowDownAttack>(this);
		attackAction_->Init();

		BossMowDownAttack* attack = dynamic_cast<BossMowDownAttack*>(attackAction_.get());
		attack->AdaptAdjustment(handType_);
		attackType_ = AttackType::MowDown_Attack;
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　攻撃の分岐
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseBossHand::Attack() {
	attackAction_->Attack();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　衝突判定
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseBossHand::OnCollisionEnter([[maybe_unused]] MeshCollider& other) {
	if (other.GetTag() == "field") {
		isGroundSlap_ = true;
		beforeAttackPos_ = transform_->GetTranslation();

		meshCollider_->SetSubTag("attacked");

		if (attackAction_ != nullptr) {
			if (attackType_ != AttackType::MowDown_Attack) {
				attackAction_->SetMoveTime(0.0f);
			}
		}

		if (attackType_ == AttackType::GooSlap_Attack) {
			if (animetor_ != nullptr) {
				animetor_->SetTransitionAnimation("slam", "stand_by");
				animationTransitionTime_ = 1.0f;
			}
		}


	} else if (other.GetTag() == "throwMissile") {
		--hp_;
		if (hp_ <= 0) {
			isAlive_ = false;
		}
	}
}

void BaseBossHand::OnCollisionStay([[maybe_unused]] MeshCollider& other) {
}

void BaseBossHand::OnCollisionExit([[maybe_unused]] MeshCollider& other) {
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