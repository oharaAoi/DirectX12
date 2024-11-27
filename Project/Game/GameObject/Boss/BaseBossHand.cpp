#include "BaseBossHand.h"
#include "Engine/Math/Easing.h"
#include "Game/Action/BossGooAttack.h"
#include "Game/Action/BossParAttack.h"
#include "Game/Action/BossMowDownAttack.h"
#include "Game/Manager/GameObjectManager.h"
#include "Game/Manager/AnimetionEffectManager.h"
#include "Engine/Audio/AudioPlayer.h"

BaseBossHand::BaseBossHand() {}
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
	// ↓ 影の初期化
	// -------------------------------------------------
	shadow_ = std::make_unique<BaseGameObject>();
	shadow_->Init();
	shadow_->SetObject("planeSphere.obj");
	shadow_->GetTransform()->SetQuaternion(Quaternion::AngleAxis(-180.0f * toRadian, Vector3::RIGHT()));
	shadow_->GetTransform()->SetScale(Vector3(3.5f, 3.5f, 3.5f));
	shadow_->SetColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	
	// -------------------------------------------------
	// ↓ Spriteの初期化
	// -------------------------------------------------

	bossHandHpUI_ = std::make_unique<BossHandHpUI>();
	bossHandHpUI_->Init(this);

	dangerUI_ = Engine::CreateSprite("danger.png");
	dangerGaugeUI_ = Engine::CreateSprite("dangerGauge.png");

	// -------------------------------------------------
	// ↓ メンバ変数の初期化
	// -------------------------------------------------
	hpSpriteDisplayTime_ = 0.0f;

	revivalTime_ = 0.0f;
	revivalMoveTime_ = 0.0f;
	
	isGroundSlap_ = false;
	isRevival_ = false;

	isAlive_ = true;

	hp_ = kDurability_;
	preHp_ = hp_;

	nowAnimatonName_ = "stand_by";
}

void BaseBossHand::Update() {
	if (!isAlive_ && !isExplosion_) {
		Revival();
		return;
	}

	// 爆発していたら
	if (isExplosion_) {
		Explosion();
		revivalTime_ = 0.0f;
		revivalMoveTime_ = 0.0f;
		return;
	}

	/*if (isAttackMove_) {
		AttackMove(transform_.get());
	}*/

	if (animetor_ != nullptr) {
		animetor_->UpdateScript(animationTime_, animationTransitionTime_);
	}

	// -------------------------------------------------
	// ↓ Spriteの処理
	// -------------------------------------------------
	// Spriteの処理
	hpSpriteDisplayTime_ -= GameTimer::DeltaTime();
	hpSpriteDisplayTime_ = std::clamp(hpSpriteDisplayTime_, 0.0f, hpSpriteDisplayTimeLimit_);
	
	// hpゲージを減らす
	bossHandHpUI_->Update((float)hp_, (float)kDurability_);

	// -------------------------------------------------
	// ↓ Objectの更新
	// -------------------------------------------------
	BaseGameObject::Update();

	// 危険マーク
	if (isDanderDraw_) {
		Vector3 dangerUIPos = worldPos_;
		if (attackType_ == AttackType::GooSlap_Attack || attackType_ == AttackType::ParSlap_Attack) {
			dangerUIPos.y = 1.0f;
		} 
		Vector3 handScrennPos = Transform(Vector3::ZERO(), dangerUIPos.MakeTranslateMat() * Render::GetVpvpMat());

		if (attackType_ == AttackType::MowDown_Attack) {
			if (handType_ == HandType::LEFT_HAND) {
				handScrennPos.x = 1100.0f;
			} else {
				handScrennPos.x = 100.0f;
			}
		}
		dangerUI_->SetTranslate(Vector2(handScrennPos.x, handScrennPos.y));
		dangerGaugeUI_->SetTranslate(dangerUI_->GetTranslate());

		dangerUI_->Update();
		dangerGaugeUI_->Update();
	}

	Vector3 handPos = worldPos_;
	handPos.y = 0.01f;
	shadow_->GetTransform()->SetTranslaion(handPos);
	shadow_->Update();

	// -------------------------------------------------
	// ↓ 更新後に行う処理
	// -------------------------------------------------
	if (hp_ != preHp_) {
		hpSpriteDisplayTime_ = hpSpriteDisplayTimeLimit_;
	}
	preHp_ = hp_;
}

void BaseBossHand::Draw() const {
	if (isAlive_ || isExplosion_ || isRevival_) {
		Engine::SetPipeline(PipelineType::NormalPipeline);
		BaseGameObject::Draw();

		shadow_->Draw();
	}
}

void BaseBossHand::DrawUI() const {
	if (!isAlive_) {
		return;
	}

	if(hpSpriteDisplayTime_ > 0.0f){
		bossHandHpUI_->Draw();
	}
}

void BaseBossHand::Draw2dUI() const {
	if (!isAlive_) {
		return;
	}

	if (isDanderDraw_) {
		dangerUI_->Draw();
		dangerGaugeUI_->Draw();
	}
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
// ↓　倒れる演出
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseBossHand::Explosion() {
	fallVelocity_ += fallAcceleration_ * GameTimer::DeltaTime();
	Vector3 handPos = transform_->GetTranslation();
	handPos += fallVelocity_ * GameTimer::DeltaTime();
	transform_->SetTranslaion(handPos);

	BaseGameObject::Update();

	if (handPos.y <= -8.0f) {
		isExplosion_ = false;
		animetor_->SetTransitionAnimation(nowAnimatonName_, "stand_by");
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　復活
//////////////////////////////////////////////////////////////////////////////////////////////////
void BaseBossHand::Revival() {
	isRevival_ = true;
	if (revivalTime_ < revivalTimeLimit_) {
		revivalTime_ += GameTimer::DeltaTime();
	} else {
		revivalMoveTime_ += GameTimer::DeltaTime();
		float t = revivalMoveTime_ / 3.0f;

		Vector3 initRevaival = initPos_;
		initRevaival.y -= 10.0f;
		Vector3 handPos = Vector3::Lerp(initRevaival, initPos_, Ease::InOut::Back(t));
		transform_->SetTranslaion(handPos);

		if (t >= 1.0f) {
			isAlive_ = true;
			isRevival_ = false;
			hp_ = kDurability_;
			preHp_ = hp_;
		}

		if (animetor_ != nullptr) {
			animetor_->UpdateScript(animationTime_, animationTransitionTime_);
		}

		BaseGameObject::Update();
	}
}

void BaseBossHand::SetDeth() {
	isGroundSlap_ = false;
	isAlive_ = false;
	isExplosion_ = true;
	isDanderDraw_ = false;
	fallVelocity_ = { 0.0f, 3.0f, 0.0f };
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　衝突判定
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseBossHand::OnCollisionEnter([[maybe_unused]] MeshCollider& other) {
	if (other.GetTag() == "field") {
		isGroundSlap_ = true;
		beforeAttackPos_ = transform_->GetTranslation();

		// SE/アニメーションを追加
		if (attackType_ == AttackType::GooSlap_Attack || attackType_ == AttackType::ParSlap_Attack) {
			AudioPlayer::SinglShotPlay("slap.mp3", 0.4f);

			Vector3 effectPos = worldPos_;
			effectPos.y = 0.0f;
			AnimetionEffectManager::AddListEffect("./Game/Resources/Model/Hand_Effect/", "Hand_Effect.gltf",
												  nullptr, false, Vector3(2.0f, 2.0f, 2.0f), Quaternion(), effectPos);
		}

		meshCollider_->SetSubTag("attacked");

		if (attackAction_ != nullptr) {
			if (attackType_ != AttackType::MowDown_Attack) {
				attackAction_->SetMoveTime(0.0f);
				/*Vector3 pos = worldPos_;
				pos.y += 18.0f;
				GameObjectManager::PopBomb(pos, Vector3(0.0f, -0.4f, 0.0f));*/
			} else {
				AudioPlayer::SinglShotPlay("mowDown.mp3", 0.3f);
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
			SetDeth();
		}


	} else if (other.GetTag() == "player") {
		Player* player = dynamic_cast<Player*>(other.GetOwner());
		if (player->GetWireTip()->GetIsBossAttack()) {
			--hp_;
			if (hp_ <= 0) {
				SetDeth();
			}
		}

	} else if (other.GetTag() == "bomb") {
		if (meshCollider_->GetSubTag() != "wait_hand") {
			--hp_;
			if (hp_ <= 0) {
				SetDeth();
			}
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