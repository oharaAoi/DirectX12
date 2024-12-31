#include "NormalEnemy.h"
#include "Engine/Editer/Window/EditerWindows.h"
#include "Game/Collider/ColliderTags.h"
#include "Engine/Utilities/BitChecker.h"
#include "Game/GameObject/Enemy/State/NormalEnemyDefaultState.h"
#include "Game/GameObject/Enemy/State/NormalEnemyMoveState.h"
#include "Game/GameObject/Enemy/State/NormalEnemyAttackState.h"

NormalEnemy::NormalEnemy() {}
NormalEnemy::~NormalEnemy() {}
void NormalEnemy::Finalize() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void NormalEnemy::Init() {
	BaseGameObject::Init();
	SetObject("enemy.gltf");
	SetAnimater("./Game/Resources/Models/Enemy/", "enemy.gltf", true, true, false);
	SetIsLighting(false);
	
	SetCollider(ColliderTags::Enemy::DEFAULT, ColliderShape::SPHERE);
	collider_->SetCollisionEnter([this](ICollider& other) { OnCollisionEnter(other); });

	// 状態の設定
	behaviorRequest_ = EnemyBehavior::DEFAULT;
	CheckBehaviorRequest();

	isDead_ = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void NormalEnemy::Update() {
	state_->Update();
	CheckBehaviorRequest();

	BaseGameObject::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void NormalEnemy::Draw() const {
	BaseGameObject::Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　状態変化のリクエストを確認する
//////////////////////////////////////////////////////////////////////////////////////////////////

void NormalEnemy::CheckBehaviorRequest() {
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();

		switch (behavior_) {
		case EnemyBehavior::DEFAULT:
			SetBehaviorState(std::make_unique<NormalEnemyDefaultState>(this));
			collider_->SetTag(ColliderTags::Enemy::DEFAULT);
			break;
		case EnemyBehavior::MOVE:
			SetBehaviorState(std::make_unique<NormalEnemyMoveState>(this));
			collider_->SetTag(ColliderTags::Enemy::DEFAULT);
			break;
		case EnemyBehavior::ATTACK:
			SetBehaviorState(std::make_unique<NormalEnemyAttackState>(this));
			collider_->SetTag(ColliderTags::Enemy::ATTACK);
			break;
		default:
			break;
		}
		// 振る舞いをリセット
		behaviorRequest_ = std::nullopt;
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

void NormalEnemy::OnCollisionEnter([[maybe_unused]] ICollider& other) {
	if (CheckBit(other.GetTag(), ColliderTags::Player::ATTACK, CheckType::EQUAL)) {
		isDead_ = true;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　連続
//////////////////////////////////////////////////////////////////////////////////////////////////

void NormalEnemy::OnCollisionStay([[maybe_unused]] ICollider& other) {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　編集処理
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void NormalEnemy::Debug_Gui() {
	BaseGameObject::Debug_Gui();
}
#endif