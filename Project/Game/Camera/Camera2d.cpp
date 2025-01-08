#include "Camera2d.h"
#include "Enviroment.h"
#include "Engine/Render.h"

Camera2d::Camera2d() {}
Camera2d::~Camera2d() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Camera2d::Init() {
	projectionMatrix_ = Matrix4x4::MakeOrthograhic(0.0f, 0.0f, float(kWindowWidth_), float(kWindowHeight_), near_, far_);
	viewMatrix_ = Matrix4x4::MakeUnit();

	viewportMatrix_ = Matrix4x4::MakeViewport(0, 0, kWindowWidth_, kWindowHeight_, 0, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Camera2d::Update() {
	// 4x4で表現するためz成分を削除
	translate_.z = 0;
	viewMatrix_ = Inverse(translate_.MakeTranslateMat());
	worldMat_ = Multiply(Multiply(Matrix4x4::MakeUnit(), Matrix4x4::MakeUnit()), translate_.MakeTranslateMat());
	// Renderに設定
	Render::SetViewProjection2D(viewMatrix_, projectionMatrix_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　編集処理
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void Camera2d::Debug_Gui() {
	ImGui::DragFloat("near", &near_, 0.1f);
	ImGui::DragFloat("far", &far_, 0.1f);
	ImGui::DragFloat2("translate", &translate_.x, 1.0f);

	projectionMatrix_ = Matrix4x4::MakeOrthograhic(0.0f, 0.0f, float(kWindowWidth_), float(kWindowHeight_), near_, far_);
}
#endif // _DEBUG