#include "FollowCamera.h"
#include "Engine/Input/Input.h"
#include "Engine/Editer/Window/EditerWindows.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Math/MyMatrix.h"

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
		{0 , 0, 0.0f},
		{0, 2, -10}
	};

	cameraMatrix_ = Multiply(Multiply(scaleMat_, rotateMat_), translateMat_);
	viewMatrix_ = Inverse(cameraMatrix_);

#ifdef _DEBUG
	EditerWindows::AddObjectWindow(std::bind(&FollowCamera::Debug_Gui, this), "followCamera");
#endif // _DEBUG
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新
//////////////////////////////////////////////////////////////////////////////////////////////////

void FollowCamera::Update() {
	RotateCamera();

	Vector3 offset = CalcucOffset();
	if (target_ != nullptr) {
		transform_.translate = target_->GetTranslation() + offset;
	}

	BaseCamera::Update();
}

void FollowCamera::RotateCamera() {
	Vector2 inputJoyStateR = Input::GetRightJoyStick();

	const float speed = 0.1f;

	destinationAngleY_ += inputJoyStateR.x * speed;

	transform_.rotate.y = LerpShortAngle(transform_.rotate.y, destinationAngleY_, 0.1f);
}

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
	ImGui::DragFloat3("translate", &transform_.translate.x, 0.1f);
	ImGui::DragFloat3("offset", &information_.offset.x, 0.1f);

	if (ImGui::Button("Save")) {
		AdjustmentItem::Save("FollowCamera", information_.ToJson("information"));
	}
}
#endif