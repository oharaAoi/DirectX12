#pragma once
#include "Engine/GameObject/BaseGameObject.h"

class BossEye : public BaseGameObject {
public:

	BossEye();
	~BossEye();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void AdaptAdjustment();

	void Shine();

#ifdef _DEBUG
	void Debug_Gui();
#endif

	const bool GetIsShine() const { return isShine_; }
	void SetIsShine(bool isShine) { isShine_ = isShine; }

private:

	std::string groupName_ = "boss_eye";

	float alpha_ = 0.0f;
	float shineTime_ = 0.0f;
	float shineTimeLimit_ = 1.0f;

	int easingType_ = 1;

	bool isShine_ = false;
};

