#include "Player.h"
#include "Engine/Editer/Window/EditerWindows.h"
#include "Game/GameObject/Player/State/PlayerDefaultState.h"
#include "Game/GameObject/Player/State/PlayerMoveState.h"
#include "Game/GameObject/Player/State/PlayerAttackState.h"
#include "Game/GameObject/Player/State/PlayerAvoidanceState.h"

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

	behaviorRequest_ = Behavior::DEFAULT;
	CheckBehaviorRequest();

	isJump_ = false;

#ifdef _DEBUG
	EditerWindows::AddObjectWindow(std::bind(&Player::Debug_Gui, this), "player");
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Update() {
	CheckAttack();
	CheckMove();
	CheckAvoidance();

	state_->Update();
	CheckBehaviorRequest();

	BaseGameObject::Update();
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
		case Behavior::ATTACK:
			SetBehaviorState(std::make_unique<PlayerAttackState>(this));
			break;
		case Behavior::AVOIDANCE:
			SetBehaviorState(std::make_unique<PlayerAvoidanceState>(this));
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
	if (isAttack_) {
		return;
	}

	if (Input::GetLeftJoyStick().x != 0.0f || Input::GetLeftJoyStick().y != 0.0f) {
		if (behavior_ == Behavior::DEFAULT) {
			behaviorRequest_ = Behavior::MOVE;
		}
	} 

	if (Input::GetIsPadTrigger(BUTTON_A)) {
		behaviorRequest_ = Behavior::MOVE;
		isJump_ = true;
		velocity_.y = 6.0f;
		acceleration_.y = -9.8f;
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
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　編集処理
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void Player::Debug_Gui() {
	BaseGameObject::Debug_Gui();
	ImGui::Separator();
	state_->Debug_Gui();
}
#endif