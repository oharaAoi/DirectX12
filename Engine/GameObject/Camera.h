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
	Matrix4x4 GetProjectionMatrix() const { return prijectionMatrix_; }

private:

	kTransform transform_;

	Matrix4x4 translateMat_;
	Matrix4x4 scaleMat_;
	Matrix4x4 rotateMat_;

	Matrix4x4 cameraMatrix_;
	Matrix4x4 prijectionMatrix_;
	Matrix4x4 viewMatrix_;
};