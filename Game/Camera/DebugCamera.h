#pragma once
#include "Engine/Math/MyMatrix.h"
#include "Engine/Math/MyMath.h"
#include "Engine/Lib/Transform.h"
#include "Engine/Input/Input.h"
#include "Engine/Manager/ImGuiManager.h"


const float kDebugCameraMoveSpeed_ = 0.05f;

/// <summary>
/// Debug用のカメラ
/// </summary>
class DebugCamera {
public:

	DebugCamera();
	~DebugCamera();

	void Init();
	void Update();
	void Draw();

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

	Matrix4x4 GetCameraMatrix() const { return cameraMatrix_; }

	Matrix4x4 GetViewMatrix() const { return viewMatrix_; }
	Matrix4x4 GetProjectionMatrix() const { return projectionMatrix_; }

	Vector3 GetTranslate() const { return transform_.translate; }
	Vector3 GetWorldTranslate() const {
		Matrix4x4 matViewInverse = Inverse(viewMatrix_);
		return { matViewInverse.m[3][0], matViewInverse.m[3][1] ,matViewInverse.m[3][2] };
	}


private:

	kTransform transform_;

	Matrix4x4 translateMat_;
	Matrix4x4 scaleMat_;
	Matrix4x4 rotateMat_;

	Matrix4x4 cameraMatrix_;
	Matrix4x4 projectionMatrix_;
	Matrix4x4 viewMatrix_;

	// ---------------------------------------------------------------
	// ↓ デバックカメラで使う変数
	// ---------------------------------------------------------------
	bool debugCameraMode_ = true;
	Vector3 lookPosition_;
	Vector2 mousePosition_;
	Vector2 rotateMousePosition_;

	bool isMove = false;

};

