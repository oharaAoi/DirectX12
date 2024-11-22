#pragma once
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Collider/Collider.h"

class Player;

class Fall
	: public BaseGameObject, public Collider {
public:

	Fall();
	~Fall();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void OnCollision([[maybe_unused]] Collider* other)override;
	void SetPlayer(Player* player) { player_ = player; }
	bool CheckMouseNear(const Matrix4x4& vpvpMat);
	bool GetNear()const { return isNear_; }
	bool GetFalling()const { return isFalling_; }
	void Reset();

#ifdef _DEBUG
	void Debug_Gui();
#endif

	const Vector3 GetForward() const { return TransformNormal(Vector3(0, 0, 1), transform_->GetWorldMatrix()); }
	const Vector3 GetWorldPos() const { return Transform(Vector3(0, 0, 0), transform_->GetWorldMatrix()); }

private:

	/// ==========================================
	/// プライベート関数
	/// ==========================================


	void OnCollisionEnter([[maybe_unused]] MeshCollider& other);
	void OnCollisionStay([[maybe_unused]] MeshCollider& other);
	void OnCollisionExit([[maybe_unused]] MeshCollider& other);


	/// ==========================================
	/// 変数
	/// ==========================================

	Player* player_ = nullptr;
	float energy_ = 0.0f;
	float canFallEnergy = 50.0f;
	bool isNear_ = false;
	bool isFalling_ = false;

	Vector3 velocity_{};
	float gravity_ = -29.4f;

};

