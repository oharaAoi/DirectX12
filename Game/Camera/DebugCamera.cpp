#include "DebugCamera.h"

DebugCamera::DebugCamera() {
	Init();
}

DebugCamera::~DebugCamera() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化
//////////////////////////////////////////////////////////////////////////////////////////////////

void DebugCamera::Finalize() {
}

void DebugCamera::Init() {
	transform_ = {
		{1.0f, 1.0f, 1.0f},
		{0 , 0, 0.0f},
		{0.0f, 5.0f, -30.0f}
	};

	// 行列の生成
	scaleMat_ = MakeScaleMatrix(transform_.scale);
	rotateMat_ = MakeRotateXYZMatrix(transform_.rotate);
	translateMat_ = MakeTranslateMatrix(transform_.translate);

	// worldの生成
	cameraMatrix_ = Multiply(Multiply(scaleMat_, rotateMat_), translateMat_);
	viewMatrix_ = Inverse(cameraMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth_) / float(kWindowHeight_), 0.1f, 100.0f);

	debugCameraMode_ = true;

	isMoveSpeed_ = 5.0f;
	moveQuaternion_ = Quaternion();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void DebugCamera::Update() {
	RotateMove();
	TransitionMove();

	quaternion_ = quaternion_.Normalize();

	scaleMat_ = MakeScaleMatrix(transform_.scale);
	rotateMat_ = quaternion_.MakeMatrix();
	translateMat_ = MakeTranslateMatrix(transform_.translate);

	cameraMatrix_ =scaleMat_ * rotateMat_ * translateMat_;
	viewMatrix_ = Inverse(cameraMatrix_);

	projectionMatrix_ = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth_) / float(kWindowHeight_), 0.1f, 100.0f);
}

#ifdef _DEBUG
void DebugCamera::Debug_Gui() {
	ImGui::Begin("Debug_Camera");
	
	Vector3 right = quaternion_.MakeRight();
	Vector3 up = quaternion_.MakeUp();
	Vector3 forward = quaternion_.MakeForward();
	Vector3 eulaer = quaternion_.ToEulerAngles();

	ImGui::DragFloat3("translate", &transform_.translate.x, 0.1f);
	ImGui::DragFloat4("rotate", &quaternion_.x, 0.01f);
	ImGui::DragFloat("isMoveSpeed", &isMoveSpeed_, 0.1f, 0.0f, 10.0f);
	ImGui::DragFloat("sensitivity", &sensitivity_, 0.1f, 0.0f, 10.0f);
	ImGui::Separator();
	ImGui::DragFloat("yaw", &yaw_, 0.1f);
	ImGui::DragFloat("pitch", &pitch_, 0.1f);
	ImGui::DragFloat4("qYaw", &qYaw.x, 0.01f);
	ImGui::DragFloat4("qPitch", &qPitch.x, 0.01f);
	ImGui::DragFloat3("eulaer", &eulaer.x, 0.01f);

	if(ImGui::Button("Reset")) {
		transform_ = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 5.0f, -30.0f} };
		quaternion_ = Quaternion();
		yaw_ = 0.0f;
		pitch_ = 0.0f;
	}

	ImGui::End();
}
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　xy軸方向移動
//////////////////////////////////////////////////////////////////////////////////////////////////

void DebugCamera::TransitionMove() {
	
	moveDirection_ = Vector3();

	if (Input::IsPressKey(DIK_A)) {
		moveDirection_ -= quaternion_.MakeRight() * isMoveSpeed_;
	}

	if (Input::IsPressKey(DIK_D)) {
		moveDirection_ += quaternion_.MakeRight() * isMoveSpeed_;
	}

	if (Input::IsPressKey(DIK_W)) {
		moveDirection_ += quaternion_.MakeForward() * isMoveSpeed_;
	}

	if (Input::IsPressKey(DIK_S)) {
		moveDirection_ -= quaternion_.MakeForward() * isMoveSpeed_;
	}

	if (Input::IsPressKey(DIK_Q)) {
		moveDirection_ += quaternion_.MakeUp() * isMoveSpeed_;
	}

	if (Input::IsPressKey(DIK_E)) {
		moveDirection_ -= quaternion_.MakeUp() * isMoveSpeed_;
	}

	if (Input::IsPressKey(DIK_LSHIFT)) {
		isMoveSpeed_ = isMoveMaxSpeed_;
	}else{
		isMoveSpeed_ = 5.0f;
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
		qYaw = Quaternion::AngleAxis(yaw_, Vector3(0.0f,1.0f,0.0f)).Normalize();

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