#pragma once
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Collider/Collider.h"


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

	

};

