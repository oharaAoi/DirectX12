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
