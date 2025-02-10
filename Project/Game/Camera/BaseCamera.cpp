#include "BaseCamera.h"
#include "Engine/Lib/Json//JsonItems.h"

BaseCamera::~BaseCamera() {Finalize();}
void BaseCamera::Finalize() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseCamera::Init() {
	transform_.scale = Vector3(1, 1, 1);
	transform_.rotate = parameter_.rotate;
	transform_.translate = parameter_.translate;

	// worldの生成
	cameraMatrix_ = transform_.MakeAffine();

	viewMatrix_ = Inverse(cameraMatrix_);
	projectionMatrix_ = Matrix4x4::MakePerspectiveFov(fovY_, float(kWindowWidth_) / float(kWindowHeight_), near_, far_);
	viewportMatrix_ = Matrix4x4::MakeViewport(0, 0, kWindowWidth_, kWindowHeight_, 0, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseCamera::Update() {
	cameraMatrix_ = transform_.MakeAffine();
	viewMatrix_ = Inverse(cameraMatrix_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　編集
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void BaseCamera::Debug_Gui(const std::string& label) {
	ImGui::DragFloat("near", &near_, 0.1f);
	ImGui::DragFloat("far", &far_, 0.1f);
	ImGui::DragFloat("fovY", &fovY_, 0.1f);
	ImGui::DragFloat3("rotate", &parameter_.rotate.x, 0.1f);
	ImGui::DragFloat3("translate", &parameter_.translate.x, 0.1f);

	transform_.rotate = parameter_.rotate;
	transform_.translate = parameter_.translate;

	if (ImGui::Button("Save")) {
		JsonItems::Save("Camera", parameter_.ToJson(label));
	}
	if (ImGui::Button("Apply")) {
		parameter_.FromJson(JsonItems::GetData("Camera", label));
	}

	projectionMatrix_ = Matrix4x4::MakePerspectiveFov(fovY_, float(kWindowWidth_) / float(kWindowHeight_), near_, far_);
}
#endif // _DEBUG

//================================================================================================//
//
// アクセッサ
//
//================================================================================================//

const Matrix4x4 BaseCamera::GetVpvpMatrix() const {
	Matrix4x4 matViewProjection = viewMatrix_ * projectionMatrix_;
	return matViewProjection * viewportMatrix_;
}

const Matrix4x4 BaseCamera::GetVPVMatrix() const {
	return viewMatrix_ * projectionMatrix_ * viewportMatrix_;
}

const Vector3 BaseCamera::GetWorldPosition() const {
	Matrix4x4 matViewInverse = Inverse(viewMatrix_);
	return { matViewInverse.m[3][0], matViewInverse.m[3][1] ,matViewInverse.m[3][2] };
}
