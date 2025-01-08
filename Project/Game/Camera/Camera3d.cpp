#include "Camera3d.h"
#include "Engine/Render.h"

Camera3d::Camera3d() {}
Camera3d::~Camera3d() {}
void Camera3d::Finalize() {}

void Camera3d::Init() {
	BaseCamera::Init();
}

void Camera3d::Update() {
	BaseCamera::Update();

	// renderの更新
	Render::SetEyePos(GetWorldPosition());
	Render::SetViewProjection(viewMatrix_, projectionMatrix_);
}

#ifdef _DEBUG
void Camera3d::Debug_Gui() {
	BaseCamera::Debug_Gui();
}
#endif