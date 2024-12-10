#pragma once
#include <memory>
#include <optional>
#include "Engine/GameObject/BaseGameObject.h"
#include "Game/Interface/ICharactorState.h"
#include "Game/Camera/FollowCamera.h"

enum class Behavior {
	DEFAULT,	// 待機状態
	MOVE,		// 移動状態
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

private:

	// ------------------- pointer ------------------- //
	FollowCamera* followCamera_ = nullptr;

	// ------------------- State関連 ------------------- //
	std::unique_ptr<ICharactorState> state_;
	Behavior behavior_ = Behavior::DEFAULT;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	// ------------------- Move関連 ------------------- //
	bool isJump_;
};

