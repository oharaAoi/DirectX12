#include "Player.h"
#include "Engine/Editer/Window/EditerWindows.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Game/Collider/ColliderTags.h"
#include "Engine/Utilities/BitChecker.h"
#include "Engine/System/Audio/AudioPlayer.h"

#include "Game/GameObject/Player/State/PlayerDefaultState.h"
#include "Game/GameObject/Player/State/PlayerMoveState.h"
#include "Game/GameObject/Player/State/PlayerJumpState.h"
#include "Game/GameObject/Player/State/PlayerAvoidanceState.h"
#include "Game/GameObject/Player/State/PlayerDamageState.h"
#include "Game/GameObject/Player/State/PlayerInputReceptionState.h"

Player::Player() {}
Player::~Player() {}

void Player::Finalize() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Init() {
	BaseGameObject::Init();
	SetObject("player.gltf");
	SetAnimater("./Game/Resources/Models/Player/", "player.gltf", true, true, false);

	// 状態の設定
	behaviorRequest_ = Behavior::DEFAULT;
	CheckBehaviorRequest();

	// statusの初期化
	status_.FromJson(AdjustmentItem::GetData(groupName_, status_.tag));
	
	hp_ = initHp_;

	// 攻撃段階の設定
	attackStep_ = AttackStep::Step_NONE;

	// Colliderの初期化
	SetCollider(ColliderTags::Player::DEFAULT, ColliderShape::SPHERE);
	collider_->SetRadius(3.0f);
	collider_->SetCollisionEnter([this](ICollider& other) { OnCollisionEnter(other); });
	collider_->SetCollisionStay([this](ICollider& other) { OnCollisionStay(other); });

	attackCollider_ = std::make_unique<AttackCollider>();
	attackCollider_->Init(ColliderTags::Player::ATTACK, ColliderShape::SPHERE);
	attackCollider_->SetRadius(4.0f);
	attackCollider_->SetCollisionEnter([this](ICollider& other) { AttackColliderOnCollisionEnter(other); });

	// 武器の初期化
	sword_ = std::make_unique<Sword>();
	sword_->Init();

	swordMat_ = animetor_->GetSkeleton()->GetSkeltonSpaceMat("mixamorig:RightHand") * transform_->GetWorldMatrix();
	sword_->GetTransform()->SetParent(swordMat_);

	// 影の初期化
	shadow_ = std::make_unique<BaseGameObject>();
	shadow_->Init();
	shadow_->SetObject("shadow.obj");
	shadow_->SetColor({ 0.0f, 0.0f, 0.0f, 0.8f });

	// フラグの初期化
	isJump_ = false;
	isAttack_ = false;
	isInvincible_ = false;


#ifdef _DEBUG
	EditerWindows::AddObjectWindow(std::bind(&Player::Debug_Gui, this), "player");
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Update() {
	CheckLockOn();

	// Stateの更新
	state_->Update();
	CheckBehaviorRequest();

	if (isInvincible_) {
		invincibleTime_ += GameTimer::DeltaTime();

		if (invincibleTime_ >= kInvincibleTime_) {
			isInvincible_ = false;
			invincibleTime_ = 0.0f;
		}
	}

	// 武器の更新
	swordMat_ = animetor_->GetSkeleton()->GetSkeltonSpaceMat("mixamorig:RightHand") * transform_->GetWorldMatrix();
	sword_->Update();

	if (isJump_) {
		if (behavior_ == Behavior::JUMP) {
			transform_->SetTranslationY(0);
			isJump_ = false;
		}
	}

	if (behavior_ == Behavior::ATTACK) {
		pTrail_->AddTrail(sword_->GetSwordTip()->GetTransform()->GetWorldMatrix(), sword_->GetSwordRoot()->GetTransform()->GetWorldMatrix());
	}

	// objectの更新
	BaseGameObject::Update();

	// 影の更新
	Vector3 pos = transform_->GetTranslation();
	pos.y = 0.05f;	// 少し浮かす
	shadow_->GetTransform()->SetTranslaion(pos);
	shadow_->Update();

	// hitStopの更新
	if (isHitStop_) {
		hitStopTimeCount_ += GameTimer::DeltaTime();
		
		if (hitStopTimeCount_ >= hitStopTime_) {
			isHitStop_ = false;
			hitStopTimeCount_ = 0.0f;
			animetor_->SetAnimationSpeed(1.0f);
		}
	}

	// attackColliderの位置を更新する
	attackCollider_->Update(QuaternionSRT{
		.scale = transform_->GetScale(),
		.rotate = transform_->GetQuaternion(),
		.translate = transform_->GetTranslation() + (transform_->GetQuaternion().Normalize() * attackColliderDiff_)
							});
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Draw() const {
	shadow_->Draw();
	sword_->Draw();
	/*swordCenter_->Draw();
	swordCenter_->Debug_Draw();*/
	
	BaseGameObject::Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　状態遷移
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::CheckBehaviorRequest() {
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();

		switch (behavior_) {
		case Behavior::DEFAULT:
			SetBehaviorState(std::make_unique<PlayerDefaultState>(this));
			break;
		case Behavior::MOVE:
			SetBehaviorState(std::make_unique<PlayerMoveState>(this));
			break;
		case Behavior::JUMP:
			SetBehaviorState(std::make_unique<PlayerJumpState>(this));
			break;
		case Behavior::ATTACK:
			SetBehaviorState(std::make_unique<PlayerAttackState>(this));
			break;
		case Behavior::AVOIDANCE:
			SetBehaviorState(std::make_unique<PlayerAvoidanceState>(this));
			break;
		case Behavior::DAMAGE:
			SetBehaviorState(std::make_unique<PlayerDamageState>(this));
			break;
		case Behavior::RECEPTION:
			SetBehaviorState(std::make_unique<PlayerInputReceptionState>(this));
			break;
		default:
			break;
		}
		// 振る舞いをリセット
		behaviorRequest_ = std::nullopt;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　移動状態受付
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::CheckMove() {
	if (Input::GetLeftJoyStick().x != 0.0f || Input::GetLeftJoyStick().y != 0.0f) {
		behaviorRequest_ = Behavior::MOVE;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　跳躍状態受付
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::CheckJump() {
	if (Input::GetIsPadTrigger(BUTTON_A)) {
		behaviorRequest_ = Behavior::JUMP;
		isJump_ = true;

		AudioPlayer::SinglShotPlay("jumpSE.mp3", 0.6f);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　攻撃受付状態
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::CheckAttack() {
	if (Input::GetIsPadTrigger(BUTTON_X)) {
		isAttack_ = true;
		behaviorRequest_ = Behavior::ATTACK;
		attackStep_ = AttackStep::Step_FIRST;
		AudioPlayer::SinglShotPlay("attack1.mp3", 0.6f);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　回避受付状態
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::CheckAvoidance() {
	if (Input::GetIsPadTrigger(BUTTON_B)) {
		behaviorRequest_ = Behavior::AVOIDANCE;
		isAvoidance_ = true;

		AudioPlayer::SinglShotPlay("rollSE.mp3", 0.6f);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　combを進める
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::CombStepUp() {
	attackStep_ = AttackStep(attackStep_ + 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Trailのリセット
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::ResetTrail() {
	pTrail_->ResetTrail();
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Z注視受付状態
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::CheckLockOn() {
	if (Input::GetIsPadTrigger(XInputButtons::R_SHOULDER)) {
		lockOn_->LockOnTarget();
		
		AudioPlayer::SinglShotPlay("lockOnSE.mp3", 0.4f);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　回避受付状態
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::LookTarget() {
	if (lockOn_->GetIsLockOn()) {
		Vector3 sub = lockOn_->GetTransform()->GetTranslation() - transform_->GetTranslation();
		float angle = std::atan2f(sub.x, sub.z);
		Quaternion lookRotate = Quaternion::AngleAxis(angle, Vector3::UP());
		transform_->SetQuaternion(Quaternion::Slerp(transform_->GetQuaternion(), lookRotate, 0.1f));
	}
}

//================================================================================================//
//
// 当たり判定
//
//================================================================================================//

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　一番最初
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::OnCollisionEnter([[maybe_unused]] ICollider& other) {
	// 回避中は取らない
	if (behavior_ == Behavior::AVOIDANCE) {
		return;
	}

	// 無敵中も取らない
	if (isInvincible_) {
		return;
	}

	if (CheckBit(other.GetTag(), ColliderTags::Enemy::ATTACK, CheckType::EQUAL)) {
		SetBehaviorRequest(Behavior::DAMAGE);
		knockBackVelocity_ = (transform_->GetTranslation() - other.GetCenterPos()).Normalize();
		isAttack_ = false;

		isInvincible_ = true;
		invincibleTime_ = 0.0f;
		hp_--;

		followCamera_->SetIsShake();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　連続
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::OnCollisionStay([[maybe_unused]] ICollider& other) {
	
}

void Player::AttackColliderOnCollisionEnter(ICollider& other) {
	if (CheckBit(other.GetTag(), ColliderTags::Enemy::DEFAULT, CheckType::EQUAL)
		|| CheckBit(other.GetTag(), ColliderTags::Enemy::ATTACK, CheckType::EQUAL)) {
		isHitStop_ = true;
		animetor_->SetAnimationSpeed(hitStopAnimationSpeed_);
	}
}

//================================================================================================//
//
// Debub系
//
//================================================================================================//


//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　編集処理
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void Player::Debug_Gui() {
	if (ImGui::CollapsingHeader("Base")) {
		BaseGameObject::Debug_Gui();
	}
	
	// statusの編集
	if (ImGui::CollapsingHeader("status")) {
		ImGui::SliderFloat("hp", &status_.hp_, 0.0f, initHp_);
		if (ImGui::Button("Save")) {
			AdjustmentItem::Save(groupName_, status_.ToJson(status_.tag));
		}
	}

	// stateの編集
	if (ImGui::CollapsingHeader("state")) {
		ImGui::Checkbox("isStateDebug", &stateDebug_);
		state_->Debug_Gui();
	}

	ImGui::DragFloat3("attackColliderDiff", &attackColliderDiff_.x, 0.1f);

	// 攻撃時の当たり判定の修正
	if (ImGui::CollapsingHeader("attackCollider")) {

	}

	sword_->Debug_Gui();
}

void Player::Debug_Draw() {
	collider_->Draw();
	if (isAttack_) {
		attackCollider_->Draw();
	}
}
#endif