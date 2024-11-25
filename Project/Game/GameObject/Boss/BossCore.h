#pragma once
#include <string>
#include <memory>
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Collider/MeshCollider.h"
#include "Game/GameObject/State/BaseObjectState.h"


enum class CoreState {
	Default,
	Hide,
	Appear,
	GameStart,
};


/// <summary>
/// Bossの攻撃をする本体
/// </summary>
class BossCore : public BaseGameObject {
public:

	BossCore();
	~BossCore();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void AdaptAdjustment();

	void CheckRequest();
	void SetBehaviorRequest(const CoreState& request) { behaviorRequest_ = request; }
	void SetBehaviorState(std::unique_ptr<BaseObjectState> behaviorState) { state_ = std::move(behaviorState); }
	void ChangeHide();

	bool CheckMouseNear(const Matrix4x4& vpvpMat);
	void SetPlayerPullBack(bool is) { isPlayerPullBack_ = is; }
	bool SetFalsePlayerPullBack();
	void SetNear(bool is) { isNear_ = is; }
	bool GetNear()const { return isNear_; }

#ifdef _DEBUG
	void Debug_Gui();

	void Debug_Draw();
#endif

	MeshCollider* GetMeshCollider() { return meshCollider_.get(); }

	// BossのHpの取得・設定
	const float GetHp() const { return hp_; }
	void SetHp(float newHp) { hp_ = newHp; };

	// 一番最初に攻撃されてか
	const bool GetIsFirstHit() const { return isFirstHit_; }
	void SetIsFirstHit(bool isFirstHit) { isFirstHit_ = isFirstHit; }

private:

	void OnCollisionEnter([[maybe_unused]] MeshCollider& other);
	void OnCollisionStay([[maybe_unused]] MeshCollider& other);
	void OnCollisionExit([[maybe_unused]] MeshCollider& other);

private:

	std::string groupName_ = "BossCore";
	bool isPlayerPullBack_ = false;

	Vector3 defaultPosition_{};
	Vector3 middlePosition_{};
	Vector3 endPosition_{};
	float moveTime_ = 0.0f;

	float energy_ = 0.0f;
	float canFallEnergy = 50.0f;
	bool isNear_ = false;
	bool isFalling_ = false;
	bool isFalsePullBack_ = false;

	float hp_ = 100.0f;
	float decrementHp_ = 10.0f;

	bool isFirstHit_;


	std::unique_ptr<BaseObjectState> state_;
	// stateパターンに関する変数
	CoreState behavior_ = CoreState::Default;
	std::optional<CoreState> behaviorRequest_ = std::nullopt;

};

