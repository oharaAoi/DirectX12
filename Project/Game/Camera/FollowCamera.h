#pragma once
#include "Game/Camera/BaseCamera.h"


class FollowCamera : public BaseCamera {
public:
	FollowCamera();
	~FollowCamera()override;

	void Finalize() override;
	void Init() override;
	void Update() override;

	void SetShakeTime(float t) { shakeTime_ = t; }
	void SetStrngth(float str) { strng = str; }

#ifdef _DEBUG
	void Debug_Gui() override;
#endif

private:

	Vector3 offset_;
	float shakeTime_ = 0.0f;

	float strng = 5.0f;

};
