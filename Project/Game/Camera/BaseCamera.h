#pragma once
#include "Engine/Math/MyMatrix.h"
#include "Engine/Math/MyMath.h"
#include "Engine/Math/MathStructures.h"
#include "Engine/Assets/WorldTransform.h"
#include "Enviroment.h"
#include "Engine/Lib/GameTimer.h"

/// <summary>
/// BaseとなるCamera
/// </summary>
class BaseCamera {
public:

	BaseCamera() = default;
	virtual ~BaseCamera();

	virtual void Finalize();
	virtual void Init();
	virtual void Update();

#ifdef _DEBUG
	virtual void Debug_Gui();
#endif // _DEBUG

public:	// アクセッサ

	const Matrix4x4& GetCameraMatrix() const { return cameraMatrix_; }

	Matrix4x4 GetViewMatrix() const { return viewMatrix_; }
	Matrix4x4 GetProjectionMatrix() const { return projectionMatrix_; }

	const Matrix4x4 GetVpvpMatrix() const;
	const Matrix4x4 GetVPVMatrix() const;
	const Vector3 GetWorldPosition() const;

protected:

	QuaternionSRT transform_;

	Matrix4x4 cameraMatrix_;
	Matrix4x4 projectionMatrix_;
	Matrix4x4 viewMatrix_;
	Matrix4x4 viewportMatrix_;

	float fovY_ = 0.45f;
	float near_ = 0.1f;
	float far_ = 100.0f;
};

