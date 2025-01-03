#pragma once
#include <memory>
#include <optional>
// engine
#include "Engine/Lib/IJsonConverter.h"
#include "Engine/Assets/GameObject/BaseGameObject.h"
// game
#include "Game/Interface/ICharactorState.h"
#include "Game/GameObject/Player/State/PlayerAttackState.h"
#include "Game/Camera/FollowCamera.h"
#include "Game/Collider/AttackCollider.h"
#include "Game/System/LockOn/LockOn.h"

enum class Behavior {
	DEFAULT,	// 待機状態
	MOVE,		// 移動状態
	JUMP,		// 跳躍状態
	ATTACK,		// 攻撃状態
	AVOIDANCE,	// 回避状態
	DAMAGE,		// 被弾状態
	RECEPTION,	// 入力受付状態
};

class Player : public BaseGameObject {
public:

	struct PlayerStatus : public IJsonConverter {
		const std::string tag = "status";
		float hp_;

		json ToJson(const std::string id) const override {
			return JsonBuilder(id)
				.Add("hp", hp_)
				.Build();
		}

		void FromJson(const json& jsonData) override {
			fromJson(jsonData, "hp", hp_);
		}
	};

public:

	Player();
	~Player();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	/// <summary>
	/// 移動入力があるかを判定する
	/// </summary>
	void CheckMove();

	/// <summary>
	/// 跳躍入力があるかを判定する
	/// </summary>
	void CheckJump();

	/// <summary>
	/// 攻撃入力があるかを判定する
	/// </summary>
	void CheckAttack();

	/// <summary>
	/// 回避入力があるかを判定する
	/// </summary>
	void CheckAvoidance();

	/// <summary>
	/// LockOnした敵の方向を向く
	/// </summary>
	void LookTarget();

	/// <summary>
	/// コンボを進める
	/// </summary>
	void CombStepUp();

private:

	void CheckLockOn();

	/// <summary>
	/// 状態変化のリクエストを確認する
	/// </summary>
	void CheckBehaviorRequest();

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

	// lockOnの設定
	void SetLockOn(LockOn* lockOn) { lockOn_ = lockOn; }

	// Jumpフラグの設定と取得
	void SetIsJump(bool isJump) { isJump_ = isJump; }
	const bool GetIsJump() const { return isJump_; }

	// 攻撃フラグの設定と取得
	void SetIsAttack(bool isAttack) { isAttack_ = isAttack; }
	const bool GetIsAttack() const { return isAttack_; }

	// 回避フラグの設定と取得
	void SetIsAvoidance(bool isAvoidance) { isAvoidance_ = isAvoidance; }
	const bool GetIsAvoidance() const { return isAvoidance_; }

	// 入力を受け付ける時間の設定と取得
	void SetInputReceptionFrame(float frame) { inputReceptionFrame_ = frame; }
	const float GetInputReceptionFrame() const { return inputReceptionFrame_; }

	// 攻撃時のColliderの取得
	AttackCollider* GetAttackCollider() { return attackCollider_.get(); }

	// knockBackする方向
	const Vector3 GetKnockBackVelocity() const { return knockBackVelocity_; }

	// 攻撃の段階の設定と取得
	void SetAttackStep(const AttackStep& step) { attackStep_ = step; }
	const AttackStep GetAttackStep() const { return attackStep_; }

	// hpの取得
	const float GetHp() const { return status_.hp_; }

private:

	const std::string groupName_ = "player";

	// ------------------- pointer ------------------- //
	FollowCamera* followCamera_ = nullptr;

	LockOn* lockOn_ = nullptr;

	// ------------------- status関連 ------------------- //
	PlayerStatus status_;

	float initHp_;

	// ------------------- State関連 ------------------- //
	std::unique_ptr<ICharactorState> state_;
	Behavior behavior_ = Behavior::DEFAULT;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	// ------------------- Move関連 ------------------- //
	bool isJump_ = false;

	Vector3 knockBackVelocity_;

	// ------------------- Avoidance関連 ------------------- //
	bool isAvoidance_ = false;

	// ------------------- Attack関連 ------------------- //
	std::unique_ptr<AttackCollider> attackCollider_;

	Vector3 attackColliderDiff_ = {0,0,4.0f};

	AttackStep attackStep_;

	bool isAttack_ = false;

	float inputReceptionFrame_;	// 入力を受け付ける時間

	// 武器
	std::unique_ptr<BaseGameObject> sword_;
	Matrix4x4 swordMat_;

	// ------------------- Debug用の変数 ------------------- //
#ifdef _DEBUG
	bool stateDebug_ = false;
#endif
};

