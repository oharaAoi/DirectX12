#pragma once
#include <memory>
#include <optional>
#include "Engine/GameObject/BaseGameObject.h"
#include "Game/Interface/ICharactorState.h"
#include "Game/Camera/FollowCamera.h"
#include "Game/Collider/AttackCollider.h"

enum class Behavior {
	DEFAULT,	// 待機状態
	MOVE,		// 移動状態
	ATTACK,		// 攻撃状態
	AVOIDANCE,	// 回避状態
};

class Player : public BaseGameObject {
public:

	Player();
	~Player();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

private:

	/// <summary>
	/// 状態変化のリクエストを確認する
	/// </summary>
	void CheckBehaviorRequest();

	/// <summary>
	/// 移動入力があるかを判定する
	/// </summary>
	void CheckMove();

	/// <summary>
	/// 攻撃入力があるかを判定する
	/// </summary>
	void CheckAttack();

	/// <summary>
	/// 回避入力があるかを判定する
	/// </summary>
	void CheckAvoidance();

	// 衝突時の処理
	void OnCollisionEnter([[maybe_unused]] ICollider& other);
	void OnCollisionStay([[maybe_unused]] ICollider& other);

#ifdef _DEBUG
	void Debug_Gui();

public:

	void Debug_Draw();

	const bool GetIsStateDebug() const { return stateDebug_; }
#endif

public:

	// 状態の設定
	void SetBehaviorRequest(const Behavior& request) { behaviorRequest_ = request; }
	void SetBehaviorState(std::unique_ptr<ICharactorState> behaviorState) { state_ = std::move(behaviorState); }

	// FollowCameraの取得と設定
	void SetFollowCamera(FollowCamera* followCamera) { followCamera_ = followCamera; };
	FollowCamera* GetFollowCamera() { return followCamera_; }

	// Jumpフラグの設定と取得
	void SetIsJump(bool isJump) { isJump_ = isJump; }
	const bool GetIsJump() const { return isJump_; }

	// 攻撃フラグの設定と取得
	void SetIsAttack(bool isAttack) { isAttack_ = isAttack; }
	const bool GetIsAttack() const { return isAttack_; }

	// 速度の設定と取得
	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }
	const Vector3 GetVelocity() const { return velocity_;}

	// 加速度の設定と取得
	void SetAcceleration(const Vector3& acceleration) { acceleration_ = acceleration; }
	const Vector3 GetAcceleration() const { return acceleration_; }

	// 攻撃時のColliderの取得
	AttackCollider* GetAttackCollider() { return attackCollider_.get(); }

private:

	// ------------------- pointer ------------------- //
	FollowCamera* followCamera_ = nullptr;

	// ------------------- State関連 ------------------- //
	std::unique_ptr<ICharactorState> state_;
	Behavior behavior_ = Behavior::DEFAULT;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	// ------------------- Move関連 ------------------- //
	bool isJump_;

	Vector3 velocity_;
	Vector3 acceleration_;

	// ------------------- Attack関連 ------------------- //
	std::unique_ptr<AttackCollider> attackCollider_;

	Vector3 attackColliderDiff_ = {0,0,4.0f};

	bool isAttack_ = false;

	// ------------------- Debug用の変数 ------------------- //
#ifdef _DEBUG
	bool stateDebug_ = false;
#endif
};

