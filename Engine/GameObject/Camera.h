#pragma once
#include "MyMatrix.h"
#include "MyMath.h"
#include "Transform.h"

class Camera {
public:

	Camera();
	~Camera();

	void Init();
	void Update();
	void Draw();

public:

	Matrix4x4 GetViewMatrix() const { return viewMatrix_; }
	Matrix4x4 GetProjectionMatrix() const { return projectionMatrix_; }

	Matrix4x4 GetViewMatrix2D() const { return viewMatrix2D_; }
	Matrix4x4 GetProjectionMatrix2D() const { return projectionMatrix2D_; }

private:

	kTransform transform_;

	Matrix4x4 translateMat_;
	Matrix4x4 scaleMat_;
	Matrix4x4 rotateMat_;

	Matrix4x4 cameraMatrix_;
	Matrix4x4 projectionMatrix_;
	Matrix4x4 viewMatrix_;
	// 2d
	Matrix4x4 projectionMatrix2D_;
	Matrix4x4 viewMatrix2D_;
};