#pragma once
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Collider/Collider.h"

class FallStone
	: public BaseGameObject, public Collider {
public:

	FallStone();
	~FallStone();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void OnCollision([[maybe_unused]] Collider* other)override;

	void Reset();
	void SetFalling(bool is) { isFalling_ = is; }
	void SetAppear(bool is) { isAppear_ = is; }

	void SetInitPosX(float x) { 
		start_ = { x, 25.0f, 12.0f };
		end_ = { x, 19.0f, 12.0f };
	}

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

	bool isFalling_ = false;

	Vector3 velocity_{};
	float gravity_ = -29.4f;

	bool isAppear_ = false;
	float appearTime_ = 0.0f;
	Vector3 start_{};
	Vector3 end_{};
};

