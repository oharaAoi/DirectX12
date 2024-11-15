#pragma once
#include "Game/Camera/BaseCamera.h"


class FollowCamera : public BaseCamera {
public:
	FollowCamera();
	~FollowCamera()override;

	void Finalize() override;
	void Init() override;
	void Update() override;

#ifdef _DEBUG
	void Debug_Gui() override;
#endif

private:

	Vector3 offset_;

};
