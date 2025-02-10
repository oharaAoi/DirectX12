#include "DebugCamera.h"
#include "Engine/System/Editer/Window/EditerWindows.h"
#include "Engine/Render.h"

DebugCamera::DebugCamera() {}
DebugCamera::~DebugCamera() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化
//////////////////////////////////////////////////////////////////////////////////////////////////

void DebugCamera::Finalize() {
}

void DebugCamera::Init() {
	BaseCamera::Init();
	
	moveBaseSpeed_ = 5.0f;
	moveSpeed_ = moveBaseSpeed_;
	moveRotate_ = transform_.rotate;
	preMoveRotate_ = transform_.rotate;

	isFocused_ = true;
	
#ifdef _DEBUG
	EditerWindows::AddObjectWindow(std::bind(&DebugCamera::Debug_Gui, this, "debugCamera"), "debugCamera");
#endif // _DEBUG
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void DebugCamera::Update() {
	if (isFocused_) {
		RotateMove();
		TransitionMove();
	}

	moveRotate_ = moveRotate_.Normalize();
	transform_.rotate = moveRotate_;

	BaseCamera::Update();

	Render::SetEyePos(GetWorldPosition());
	Render::SetViewProjection(viewMatrix_, projectionMatrix_);
}

#ifdef _DEBUG
void DebugCamera::Debug_Gui([[maybe_unused]] const std::string& label) {
	Vector3 right = moveRotate_.MakeRight();
	Vector3 up = moveRotate_.MakeUp();
	Vector3 forward = moveRotate_.MakeForward();
	
	ImGui::DragFloat3("translate", &transform_.translate.x, 0.1f);
	ImGui::DragFloat4("rotate", &moveRotate_.x, 0.01f);
	ImGui::DragFloat("sensitivity", &sensitivity_, 0.01f, 0.0f, 0.1f);
	ImGui::Separator();
	ImGui::DragFloat4("qYaw", &qYaw.x, 0.01f);
	ImGui::DragFloat4("qPitch", &qPitch.x, 0.01f);
	
	if (ImGui::Button("Reset")) {
		transform_ = { {1.0f, 1.0f, 1.0f}, Quaternion(), {0.0f, 5.0f, -30.0f}};
		moveRotate_ = Quaternion();
		yaw_ = 0.0f;
		pitch_ = 0.0f;
	}
}
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　xy軸方向移動
//////////////////////////////////////////////////////////////////////////////////////////////////

void DebugCamera::TransitionMove() {

	moveDirection_ = Vector3();

	if (Input::IsPressKey(DIK_A)) {
		moveDirection_ -= moveRotate_.MakeRight() * moveSpeed_;
	}

	if (Input::IsPressKey(DIK_D)) {
		moveDirection_ += moveRotate_.MakeRight() * moveSpeed_;
	}

	if (Input::IsPressKey(DIK_W)) {
		moveDirection_ += moveRotate_.MakeForward() * moveSpeed_;
	}

	if (Input::IsPressKey(DIK_S)) {
		moveDirection_ -= moveRotate_.MakeForward() * moveSpeed_;
	}

	if (Input::IsPressKey(DIK_Q)) {
		moveDirection_ += moveRotate_.MakeUp() * moveSpeed_;
	}

	if (Input::IsPressKey(DIK_E)) {
		moveDirection_ -= moveRotate_.MakeUp() * moveSpeed_;
	}

	if (Input::IsPressKey(DIK_LSHIFT)) {
		moveSpeed_ = moveBaseSpeed_ * 2.0f;
	} else {
		moveSpeed_ = moveBaseSpeed_;
	}
	
	transform_.translate += moveDirection_ * GameTimer::DeltaTime();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　回転移動(FPS視点)
//////////////////////////////////////////////////////////////////////////////////////////////////

void DebugCamera::RotateMove() {
	if (Input::IsPressMouse(1)) {

		Vector2 dire = Input::GetMousePosition() - preMousePos_;

		// Y軸回転(Y軸回転は必ずworld空間での回転が行われる)
		yaw_ += dire.x * sensitivity_ * GameTimer::DeltaTime();
		qYaw = Quaternion::AngleAxis(yaw_, Vector3(0.0f, 1.0f, 0.0f)).Normalize();

		// X軸回転(X軸回転は必ずlocal空間で回転が行われる)
		pitch_ += dire.y * sensitivity_ * GameTimer::DeltaTime();
		qPitch = Quaternion::AngleAxis(pitch_, Vector3(1.0f, 0.0f, 0.0f)).Normalize();

		// 回転合成
		moveRotate_ = (qYaw * preMoveRotate_ * qPitch).Normalize();

	} else {
		preMoveRotate_ = moveRotate_;
		yaw_ = 0;
		pitch_ = 0;
	}

	preMousePos_ = Input::GetMousePosition();
}