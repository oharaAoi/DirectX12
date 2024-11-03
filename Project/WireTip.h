#pragma once
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Collider/Collider.h"


class WireTip : public BaseGameObject,public Collider {
public:
	WireTip();
	~WireTip();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void OnCollision([[maybe_unused]] Collider* other)override;
	bool GetHit()const { return hit_; }
	void SetHit(bool is) { hit_ = is; }

	bool GetSnagged()const { return isSnagged_; }
	void SetSnagged(bool is) { isSnagged_ = is; }

	const Vector3 GetWorldPos() const override { return transform_->GetTranslation(); }

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	bool hit_ = false;
	bool isSnagged_ = false;

};
