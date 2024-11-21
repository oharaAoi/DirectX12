#pragma once
#include "Engine/GameObject/BaseGameObject.h"
#include "Game/GameObject/CanThrowObject.h"
#include "Engine/Collider/Collider.h"
#include "Game/GameObject/Missile.h"

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

	bool GetNeglect()const { return isNeglect_; }
	void SetNeglect(bool is) { isNeglect_ = is; }


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

	// ボスを攻撃中かのフラグの取得・設定
	const bool GetIsBossAttack() const { return isBossAttack_; }
	void SetIsBossAttack(bool isBossAttack) { isBossAttack_ = isBossAttack; }

	const Vector3 GetWorldPos() const override { return transform_->GetTranslation(); }
	float GetWeight() { return weight_; }

	// Objectを捕まえているかのフラグ
	const bool GetIsCautchObject() const { return isCautchObject_; }
	void SetIsCautchObject(bool isCautch) { isCautchObject_ = isCautch; }

	CanThrowObject* GetCatchObject() { return catchObject_; }
	void ReleseCathcObject() { 
		catchObject_ = nullptr;
		isCautchObject_ = false;
		meshCollider_->SetTag(notCatchStateTag_);
	}

private:

	bool isNeglect_ = false;

	bool isHit_ = false;
	bool isCautch_ = false;
	bool isFollow_ = false;
	bool isSnagged_ = false;
	bool isPull_ = false;
	float weight_ = 0.0f;

	bool isBossAttack_ = false;	// ボスの手などを攻撃中か

	bool isCautchObject_ = false;
	CanThrowObject* catchObject_ = nullptr;

	const std::string notCatchStateTag_ = "notCatchWireTip";
	const std::string catchStateTag_ = "catchWireTip";

};
