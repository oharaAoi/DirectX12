#pragma once
#include "Game/Camera/BaseCamera.h"
#include "Engine/Input/Input.h"
#include "Engine/Math/Quaternion.h"

const float kDebugCameraMoveSpeed_ = 0.05f;

class DebugCamera : public BaseCamera {
public:

	DebugCamera();
	~DebugCamera() override;

	void Finalize() override;
	void Init() override;
	void Update() override;

#ifdef _DEBUG
	void Debug_Gui() override;
#endif

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

private:

	Quaternion quaternion_;
	// 回転する前のQuaternion
	Quaternion moveQuaternion_;

	// ---------------------------------------------------------------
	// ↓ デバックカメラで使う変数
	// ---------------------------------------------------------------
	bool debugCameraMode_ = true;

	float isMoveSpeed_;
	float isMoveMaxSpeed_ = 10.0f;
	Vector3 moveDirection_;
	Vector2 preMousePos_;

	float yaw_ = 0.0f; 
	float pitch_ = 0.0f;
	float sensitivity_ = 10.0f; // 回転感度
	
	bool isMove = false;

	Quaternion qYaw;
	Quaternion qPitch;
};

