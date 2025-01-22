#include "Camera3d.h"
#include "Engine/Render.h"
#include "Engine/Editer/Window/EditerWindows.h"
#include "Engine/Utilities/AdjustmentItem.h"

Camera3d::Camera3d() {}
Camera3d::~Camera3d() {}
void Camera3d::Finalize() {}

void Camera3d::Init() {
	BaseCamera::Init();

	parameter_.FromJson(AdjustmentItem::GetData("Camera", "camera3d"));
#ifdef _DEBUG
	EditerWindows::AddObjectWindow(std::bind(&Camera3d::Debug_Gui, this, "camera3d"), "camera3d");
#endif // _DEBUG
}

void Camera3d::Update() {
	BaseCamera::Update();

	// renderの更新
	Render::SetEyePos(GetWorldPosition());
	Render::SetViewProjection(viewMatrix_, projectionMatrix_);
}

#ifdef _DEBUG
void Camera3d::Debug_Gui(const std::string& label) {
	BaseCamera::Debug_Gui(label);
}
#endif