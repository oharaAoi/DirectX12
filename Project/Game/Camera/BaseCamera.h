#pragma once
#include "Engine/Math/MyMatrix.h"
#include "Engine/Math/MyMath.h"
#include "Engine/Lib/Transform.h"
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
	virtual void Debug_Gui() {};
#endif // _DEBUG

	const Matrix4x4& GetCameraMatrix() const { return cameraMatrix_; }

	Matrix4x4 GetViewMatrix() const { return viewMatrix_; }
	Matrix4x4 GetProjectionMatrix() const { return projectionMatrix_; }

	Matrix4x4 GetViewMatrix2D() const { return viewMatrix2D_; }
	Matrix4x4 GetProjectionMatrix2D() const { return projectionMatrix2D_; }

	const Matrix4x4 GetVpvpMatrix() const { return vpvpMatrix_; }

	const Matrix4x4 GetVPVMatrix() const { return viewMatrix_ * projectionMatrix_ * viewportMatrix_; }
	const Matrix4x4 GetVPV2DMatrix() const { return view2DMatrix_ * projectionMatrix_ * viewportMatrix_; }

	Vector3 GetTranslate() const { return transform_.translate; }
	Vector3 GetWorldTranslate() const {
		Matrix4x4 matViewInverse = Inverse(viewMatrix_);
		return { matViewInverse.m[3][0], matViewInverse.m[3][1] ,matViewInverse.m[3][2] };
	}

	void SetTarget(WorldTransform* target) { target_ = target; }

	void SetIsFocused(bool isFocesd) { isFocused_ = isFocesd; }
	const bool GetIsFocused() const { return isFocused_; }

protected:

	WorldTransform* target_;

	kTransform transform_;

	Matrix4x4 translateMat_;
	Matrix4x4 scaleMat_;
	Matrix4x4 rotateMat_;

	Matrix4x4 cameraMatrix_;
	Matrix4x4 projectionMatrix_;
	Matrix4x4 viewMatrix_;
	Matrix4x4 viewportMatrix_;
	Matrix4x4 vpvpMatrix_;
	// 2d
	Matrix4x4 projectionMatrix2D_;
	Matrix4x4 viewMatrix2D_;
	Matrix4x4 viewport2D_;

	Matrix4x4 view2DMatrix_;


	bool isFocused_;

};

