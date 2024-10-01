#pragma once
#include "Engine/Math/MyMath.h"
#include "Engine/Lib/Transform.h"

/// <summary>
/// Effectを作成する際に使用するカメラ
/// </summary>
class EffectSystemCamera {
public:

	EffectSystemCamera();
	~EffectSystemCamera();

	void Init();
	void Update();

public:

	const Matrix4x4 GetCameraMatrix() const { return cameraMatrix_; }
	const Matrix4x4 GetViewMatrix() const { return viewMatrix_; }
	const Matrix4x4 GetProjectionMatrix() const { return projectionMatrix_; }

private:

	kTransform transform_;

	Matrix4x4 translateMat_;
	Matrix4x4 scaleMat_;
	Matrix4x4 rotateMat_;

	Matrix4x4 cameraMatrix_;
	Matrix4x4 projectionMatrix_;
	Matrix4x4 viewMatrix_;

};

