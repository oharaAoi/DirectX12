#pragma once
#include "MyMatrix.h"
#include "MyMath.h"
#include "Transform.h"
#include "Input.h"

#ifdef _DEBUG
#include "ImGuiManager.h"
#endif // DEBUG

const float kCameraMoveSpeed_ = 0.05f;

class Camera {
public:

	Camera();
	~Camera();

	void Init();
	void Update();
	void Draw();

public:

	/// <summary>
	/// カメラを動かす
	/// </summary>
	void TransitionMove();

	/// <summary>
	/// カメラを回転させる
	/// </summary>
	void RotateMove();

	/// <summary>
	/// マウスのスクロールで移動する
	/// </summary>
	void ScrollMove();

public:

	Matrix4x4 GetViewMatrix() const { return viewMatrix_; }
	Matrix4x4 GetProjectionMatrix() const { return projectionMatrix_; }

	Matrix4x4 GetViewMatrix2D() const { return viewMatrix2D_; }
	Matrix4x4 GetProjectionMatrix2D() const { return projectionMatrix2D_; }

	Vector3 GetTranslate() const { return transform_.translate; }

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

	// ---------------------------------------------------------------
	// ↓ デバックカメラで使う変数
	// ---------------------------------------------------------------
	bool debugCameraMode_ = true;
	Vector3 lookPosition_;
	Vector2 mousePosition_;
	Vector2 rotateMousePosition_;
};