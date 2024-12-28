#include "Player.h"
#include "Engine/Editer/Window/EditerWindows.h"
#include "Game/GameObject/Player/State/PlayerDefaultState.h"
#include "Game/GameObject/Player/State/PlayerMoveState.h"
#include "Game/GameObject/Player/State/PlayerJumpState.h"
#include "Game/GameObject/Player/State/PlayerAttackState.h"
#include "Game/GameObject/Player/State/PlayerAvoidanceState.h"
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

	// Colliderの初期化
	SetCollider("player", ColliderShape::SPHERE);
	collider_->SetCollisionEnter([this](ICollider& other) { OnCollisionEnter(other); });
	collider_->SetCollisionStay([this](ICollider& other) { OnCollisionStay(other); });

	attackCollider_ = std::make_unique<AttackCollider>();
	attackCollider_->Init("playerAttackCollider", ColliderShape::SPHERE);
	attackCollider_->SetRadius(4.0f);

	// フラグの初期化
	isJump_ = false;
	isAttack_ = false;

#ifdef _DEBUG
	EditerWindows::AddObjectWindow(std::bind(&Player::Debug_Gui, this), "player");
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Update() {
	state_->Update();
	CheckBehaviorRequest();

	BaseGameObject::Update();

	// attackColliderの位置を更新する
	attackCollider_->Update(SRT{
		.scale = transform_->GetScale(),
		.rotate = transform_->GetQuaternion(),
		.translate = transform_->GetTranslation() + (transform_->GetQuaternion().Normalize() * attackColliderDiff_)
							});
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Draw() const {
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
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　攻撃受付状態
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::CheckAttack() {
	if (Input::GetIsPadTrigger(BUTTON_X)) {
		behaviorRequest_ = Behavior::ATTACK;
		isAttack_ = true;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　回避受付状態
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::CheckAvoidance() {
	if (Input::GetIsPadTrigger(BUTTON_B)) {
		behaviorRequest_ = Behavior::AVOIDANCE;
		isAvoidance_ = true;
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
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　連続
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::OnCollisionStay([[maybe_unused]] ICollider& other) {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　編集処理
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void Player::Debug_Gui() {
	BaseGameObject::Debug_Gui();
	ImGui::Separator();
	ImGui::Checkbox("isStateDebug", &stateDebug_);
	state_->Debug_Gui();

	ImGui::DragFloat3("attackColliderDiff", &attackColliderDiff_.x, 0.1f);
}

void Player::Debug_Draw() {
	attackCollider_->Draw();
}
#endif