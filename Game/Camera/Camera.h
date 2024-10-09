#pragma once
#include "Game/Camera/BaseCamera.h"

class Camera : public BaseCamera {
public:

	Camera();
	~Camera() override;

	void Finalize() override;
	void Init() override;
	void Update() override;

#ifdef _DEBUG
	void Debug_Gui() override;
#endif

private:

	Vector3 offset_;

};