#include "Camera.h"
#include "Engine/Editer/Window/EditerWindows.h"

Camera::Camera() {
	Init();
}

Camera::~Camera() {
}

void Camera::Finalize() {
}

void Camera::Init() {
	BaseCamera::Init();

	transform_ = {
		{1.0f, 1.0f, 1.0f},
		{0 , 0, 0.0f},
		{0.0f, 4.0f, -30.0f}
	};

	offset_ = transform_.translate;

#ifdef _DEBUG
	EditerWindows::AddObjectWindow(std::bind(&Camera::Debug_Gui, this), "camera");
#endif // _DEBUG
}

void Camera::Update() {
	// -------------------------------------------------
	// ↓ Targetがいたら
	// -------------------------------------------------
	if (target_ != nullptr) {
		transform_.translate.x = target_->GetTranslation().x + offset_.x;
	}

	BaseCamera::Update();
}

#ifdef _DEBUG
#include "Engine/System/Manager/ImGuiManager.h"
void Camera::Debug_Gui() {
	if (ImGui::Button("Reset")) {
		transform_ = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -9.0f} };
		transform_.rotate = { 0.0f, 0.0f, 0.0f };
		rotateMat_ = transform_.rotate.MakeRotateMat();
	}

	ImGui::DragFloat3("translate", &transform_.translate.x, 0.1f);
	ImGui::DragFloat3("rotate", &transform_.rotate.x, 0.1f);
}
#endif
