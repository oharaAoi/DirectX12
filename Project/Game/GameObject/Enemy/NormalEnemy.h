#pragma once
#include <memory>
#include <optional>
// game
#include "Game/Interface/ICharactorState.h"
#include "Engine/GameObject/BaseGameObject.h"

enum class EnemyBehavior {
	DEFAULT,
	MOVE,
	ATTACK,
};

/// <summary>
/// 基本的な敵
/// </summary>
class NormalEnemy : public BaseGameObject {
public:

	NormalEnemy();
	~NormalEnemy();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

private:

	/// <summary>
	/// 状態変化のリクエストを確認する
	/// </summary>
	void CheckBehaviorRequest();

	void OnCollisionEnter([[maybe_unused]] ICollider& other);
	void OnCollisionStay([[maybe_unused]] ICollider& other);

#ifdef _DEBUG
	void Debug_Gui();
#endif

public:

	// 状態の設定
	void SetBehaviorRequest(const EnemyBehavior& request) { behaviorRequest_ = request; }
	void SetBehaviorState(std::unique_ptr<ICharactorState> behaviorState) { state_ = std::move(behaviorState); }

	const bool GetIsDead() const { return isDead_; }

private:

	bool isDead_ = false;

	// ------------------- State関連 ------------------- //
	std::unique_ptr<ICharactorState> state_;
	EnemyBehavior behavior_ = EnemyBehavior::DEFAULT;
	std::optional<EnemyBehavior> behaviorRequest_ = std::nullopt;

};

