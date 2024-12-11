#pragma once
#include <memory>
#include <optional>
#include "Engine/GameObject/BaseGameObject.h"
#include "Game/Interface/ICharactorState.h"
#include "Game/Camera/FollowCamera.h"

enum class Behavior {
	DEFAULT,	// 待機状態
	MOVE,		// 移動状態
	ATTACK,		// 攻撃状態
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

	void CheckBehaviorRequest();

	void CheckMove();

	void CheckAttack();

#ifdef _DEBUG
	void Debug_Gui();
#endif

public:

	void SetBehaviorRequest(const Behavior& request) { behaviorRequest_ = request; }
	void SetBehaviorState(std::unique_ptr<ICharactorState> behaviorState) { state_ = std::move(behaviorState); }

	void SetFollowCamera(FollowCamera* followCamera) { followCamera_ = followCamera; };
	FollowCamera* GetFollowCamera() { return followCamera_; }

	void SetIsJump(bool isJump) { isJump_ = isJump; }
	const bool GetIsJump() const { return isJump_; }

	void SetIsAttack(bool isAttack) { isAttack_ = isAttack; }
	const bool GetIsAttack() const { return isAttack_; }

	const Vector3 GetVelocity() const { return velocity_;}
	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }

	const Vector3 GetAcceleration() const { return acceleration_; }
	void SetAcceleration(const Vector3& acceleration) { acceleration_ = acceleration; }

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
	bool isAttack_ = false;

};

