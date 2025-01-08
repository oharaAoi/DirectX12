#include "EffectSystemCamera.h"
#include "Engine/System/Input/Input.h"
#include "Engine/Render.h"
#ifdef _DEBUG
#include "Engine/System/Manager/ImGuiManager.h"
#endif // _DEBUG

EffectSystemCamera::EffectSystemCamera() { Init(); }
EffectSystemCamera::~EffectSystemCamera() {}

void EffectSystemCamera::Finalize() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////


void EffectSystemCamera::Init() {
	BaseCamera::Init();

	debugCameraMode_ = true;

	moveBaseSpeed_ = 5.0f;
	moveSpeed_ = 5.0f;
	quaternion_ = Quaternion::AngleAxis(20.0f * toRadian, Vector3::RIGHT());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystemCamera::Update() {

	RotateMove();
	TransitionMove();

	transform_.rotate = quaternion_.Normalize();

	BaseCamera::Update();

	// renderの更新
	Render::SetEyePos(GetWorldPosition());
	Render::SetViewProjection(viewMatrix_, projectionMatrix_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystemCamera::TransitionMove() {
	moveDirection_ = Vector3();

	if (Input::IsPressKey(DIK_A)) {
		moveDirection_ -= quaternion_.MakeRight() * moveSpeed_;
	}

	if (Input::IsPressKey(DIK_D)) {
		moveDirection_ += quaternion_.MakeRight() * moveSpeed_;
	}

	if (Input::IsPressKey(DIK_W)) {
		moveDirection_ += quaternion_.MakeForward() * moveSpeed_;
	}

	if (Input::IsPressKey(DIK_S)) {
		moveDirection_ -= quaternion_.MakeForward() * moveSpeed_;
	}

	if (Input::IsPressKey(DIK_Q)) {
		moveDirection_ += quaternion_.MakeUp() * moveSpeed_;
	}

	if (Input::IsPressKey(DIK_E)) {
		moveDirection_ -= quaternion_.MakeUp() * moveSpeed_;
	}

	transform_.translate += moveDirection_ * GameTimer::DeltaTime();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectSystemCamera::RotateMove() {
	if (Input::IsPressMouse(1)) {

		Vector2 dire = Input::GetMousePosition() - preMousePos_;

		// Y軸回転(Y軸回転は必ずworld空間での回転が行われる)
		yaw_ += dire.x * sensitivity_ * GameTimer::DeltaTime();
		qYaw = Quaternion::AngleAxis(yaw_, Vector3(0.0f, 1.0f, 0.0f)).Normalize();

		// X軸回転(X軸回転は必ずlocal空間で回転が行われる)
		pitch_ += dire.y * sensitivity_ * GameTimer::DeltaTime();
		qPitch = Quaternion::AngleAxis(pitch_, Vector3(1.0f, 0.0f, 0.0f)).Normalize();

		// 回転合成
		quaternion_ = (qYaw * moveQuaternion_ * qPitch).Normalize();

	} else {
		moveQuaternion_ = quaternion_;
		yaw_ = 0;
		pitch_ = 0;
	}

	preMousePos_ = Input::GetMousePosition();
}

#ifdef _DEBUG
void EffectSystemCamera::Debug_Gui() {
	ImGui::Begin("effectCamera");
	ImGui::DragFloat3("rotate", &transform_.rotate.x, 0.1f);
	ImGui::DragFloat3("translate", &transform_.translate.x, 0.1f);
	ImGui::End();
}
#endif