#pragma once
#include <string>
#include <memory>
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Collider/MeshCollider.h"

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

	bool CheckMouseNear(const Matrix4x4& vpvpMat);
	void SetPlayerPullBack(bool is) { isPlayerPullBack_ = is; }
	bool SetFalsePlayerPullBack();
	bool GetNear()const { return isNear_; }

#ifdef _DEBUG
	void Debug_Gui();

	void Debug_Draw();
#endif

	MeshCollider* GetMeshCollider() { return meshCollider_.get(); }

	// BossのHpの取得・設定
	const float GetHp() const { return hp_; }
	void SetHp(float newHp) { hp_ = newHp; };

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

};

