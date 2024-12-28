#pragma once
#include "Game/Camera/BaseCamera.h"

class FollowCamera 
	: public BaseCamera {
public:

	FollowCamera();
	~FollowCamera();

	void Finalize() override;
	void Init() override;
	void Update() override;

	void RotateCamera();

private:

	Vector3 CalcucOffset();

public:

#ifdef _DEBUG
	void Debug_Gui() override;
#endif

	const Matrix4x4& GetRotateMat() const { return rotateMat_; }

private:

	Quaternion quaternion_;
	// 回転する前のQuaternion
	Quaternion moveQuaternion_;

	Vector3 offset_;

	float destinationAngleY_;
	float destinationAngleX_;
};

