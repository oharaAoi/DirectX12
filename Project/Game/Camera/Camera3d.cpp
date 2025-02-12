#include "Camera3d.h"
#include "Engine/Render.h"
#include "Engine/System/Editer/Window/EditerWindows.h"
#include "Engine/Lib/Json/JsonItems.h"

Camera3d::Camera3d() {}
Camera3d::~Camera3d() {}
void Camera3d::Finalize() {}

void Camera3d::Init() {
	BaseCamera::Init();

	parameter_.FromJson(JsonItems::GetData("Camera", "camera3d"));
#ifdef _DEBUG
	EditerWindows::AddObjectWindow(this, "camera3d");
#endif // _DEBUG
}

void Camera3d::Update() {
	BaseCamera::Update();

	// renderの更新
	Render::SetEyePos(GetWorldPosition());
	Render::SetViewProjection(viewMatrix_, projectionMatrix_);
}

#ifdef _DEBUG
void Camera3d::Debug_Gui() {
	ImGui::DragFloat("near", &near_, 0.1f);
	ImGui::DragFloat("far", &far_, 0.1f);
	ImGui::DragFloat("fovY", &fovY_, 0.1f);
	ImGui::DragFloat3("rotate", &parameter_.rotate.x, 0.1f);
	ImGui::DragFloat3("translate", &parameter_.translate.x, 0.1f);

	transform_.rotate = parameter_.rotate;
	transform_.translate = parameter_.translate;

	if (ImGui::Button("Save")) {
		JsonItems::Save("Camera", parameter_.ToJson("camera3d"));
	}
	if (ImGui::Button("Apply")) {
		parameter_.FromJson(JsonItems::GetData("Camera", "camera3d"));
	}

	projectionMatrix_ = Matrix4x4::MakePerspectiveFov(fovY_, float(kWindowWidth_) / float(kWindowHeight_), near_, far_);
}
#endif
