#pragma once
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Collider/Collider.h"
#include "Engine/Collider/MeshCollider.h"


class WireTip : public BaseGameObject,public Collider {
public:
	WireTip();
	~WireTip();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void OnCollision([[maybe_unused]] Collider* other)override;

	void OnCollision(MeshCollider& other);

private:

	void OnCollisionEnter([[maybe_unused]] MeshCollider& other);
	void OnCollisionStay([[maybe_unused]] MeshCollider& other);
	void OnCollisionExit([[maybe_unused]] MeshCollider& other);

public:

#ifdef _DEBUG
	void Debug_Gui();
#endif

	MeshCollider* GetMeshCollider() { return meshCollider_.get(); }

	bool GetHit()const { return isHit_; }
	void SetHit(bool is) { isHit_ = is; }
	bool GetCautch()const { return isCautch_; }
	void SetCautch(bool is) { isCautch_ = is; }

	bool GetFollow()const { return isFollow_; }
	void SetFolllow(bool is) { isFollow_ = is; }

	bool GetSnagged()const { return isSnagged_; }
	void SetSnagged(bool is) { isSnagged_ = is; }

	bool GetPull()const { return isPull_; }
	void SetPull(bool is) { isPull_ = is; }

	const Vector3 GetWorldPos() const override { return transform_->GetTranslation(); }
	float GetWeight() { return weight_; }

private:

	bool isHit_ = false;
	bool isCautch_ = false;
	bool isFollow_ = false;
	bool isSnagged_ = false;
	bool isPull_ = false;
	float weight_ = 0.0f;

};
