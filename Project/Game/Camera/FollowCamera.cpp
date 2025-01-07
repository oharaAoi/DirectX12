#include "FollowCamera.h"
#include "Engine/System/Input/Input.h"
#include "Engine/Editer/Window/EditerWindows.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Math/MyMatrix.h"
#include "Engine/Math/MyRandom.h"

FollowCamera::FollowCamera() {
}

FollowCamera::~FollowCamera() {
}

void FollowCamera::Finalize() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void FollowCamera::Init() {
	BaseCamera::Init();

	information_.FromJson(AdjustmentItem::GetData("FollowCamera", "information"));

	transform_ = {
		{1.0f, 1.0f, 1.0f},
		{0.1f , 0, 0.0f},
		{0, 2, -10}
	};

	cameraMatrix_ = Multiply(Multiply(scaleMat_, rotateMat_), translateMat_);
	viewMatrix_ = Inverse(cameraMatrix_);

	lockOnDestinationAngleY_ = 0.0f;

#ifdef _DEBUG
	EditerWindows::AddObjectWindow(std::bind(&FollowCamera::Debug_Gui, this), "followCamera");
#endif // _DEBUG
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新
//////////////////////////////////////////////////////////////////////////////////////////////////

void FollowCamera::Update() {
	RotateCamera();

	if (isShake_) {
		Shake();
	}

	Vector3 offset = CalcucOffset();
	if (target_ != nullptr) {
		transform_.translate = target_->GetTranslation() + offset + shakeVelocity_;
	}

	if (lockOn_->GetIsLockOn()) {
		Vector3 sub = lockOn_->GetTransform()->GetTranslation() - target_->GetTranslation();
		lockOnDestinationAngleY_ = std::atan2f(sub.x, sub.z);
		transform_.rotate.y = LerpShortAngle(transform_.rotate.y, lockOnDestinationAngleY_, 0.05f);
	}

	BaseCamera::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　カメラを回転させる
//////////////////////////////////////////////////////////////////////////////////////////////////

void FollowCamera::RotateCamera() {
	if (lockOn_->GetIsLockOn()) {
		return;
	}

	Vector2 inputJoyStateR = Input::GetRightJoyStick();
	const float speed = 0.1f;
	destinationAngleY_ += inputJoyStateR.x * speed;
	transform_.rotate.y = LerpShortAngle(transform_.rotate.y, destinationAngleY_, 0.1f);
}

void FollowCamera::Shake() {
	shakeTime_ += GameTimer::DeltaTime();

	if (shakeTime_ >= shakeTimeLimit_) {
		isShake_ = false;
		shakeTime_ = 0.0f;
		shakeVelocity_ = Vector3();
		return;
	}

	shakeStrength_ = shakeTimeLimit_ - shakeTime_;

	shakeVelocity_ = {
		RandomFloat(-shakeStrength_, shakeStrength_),
		RandomFloat(-shakeStrength_, shakeStrength_),
		RandomFloat(-shakeStrength_, shakeStrength_)
	};
}

void FollowCamera::SetIsShake() {
	isShake_ = true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　offsetの位置を計算する
//////////////////////////////////////////////////////////////////////////////////////////////////

Vector3 FollowCamera::CalcucOffset() {
	Vector3 offset = information_.offset;
	// 回転行列の合成
	Matrix4x4 matRotate = transform_.rotate.MakeRotateMat();
	offset = TransformNormal(offset, matRotate);

	return offset;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　編集
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void FollowCamera::Debug_Gui() {
	ImGui::DragFloat3("rotate", &transform_.rotate.x, 0.1f);
	ImGui::DragFloat3("translate", &transform_.translate.x, 0.1f);
	ImGui::DragFloat3("offset", &information_.offset.x, 0.1f);

	if (ImGui::Button("Save")) {
		AdjustmentItem::Save("FollowCamera", information_.ToJson("information"));
	}
}
#endif