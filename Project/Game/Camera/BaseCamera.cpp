#include "BaseCamera.h"

BaseCamera::~BaseCamera() {Finalize();}
void BaseCamera::Finalize() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化
//////////////////////////////////////////////////////////////////////////////////////////////////

void BaseCamera::Init() {
	transform_ = {
		{1.0f, 1.0f, 1.0f},
		Quaternion::AngleAxis(25.0f * toRadian, Vector3::RIGHT()),
		{0.0f, 8.0f, -15.0f}
	};

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
void BaseCamera::Debug_Gui() {
	ImGui::DragFloat("near", &near_, 0.1f);
	ImGui::DragFloat("far", &far_, 0.1f);
	ImGui::DragFloat("fovY", &fovY_, 0.1f);
	ImGui::DragFloat2("rotate", &transform_.rotate.x, 1.0f);
	ImGui::DragFloat2("translate", &transform_.translate.x, 1.0f);

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
