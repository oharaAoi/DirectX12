#include "FollowCamera.h"

FollowCamera::FollowCamera() {
	Init();
}

FollowCamera::~FollowCamera() {
}

void FollowCamera::Finalize() {
}

void FollowCamera::Init() {
	BaseCamera::Init();

	transform_ = {
		{1.0f, 1.0f, 1.0f},
		{-0.1f , 0, 0.0f},
		{0.0f, 2.0f, -30.0f}
	};

	offset_ = transform_.translate;
}

void FollowCamera::Update() {
	// -------------------------------------------------
	// ↓ Targetがいたら
	// -------------------------------------------------
	if (target_ != nullptr) {
		transform_.translate.x = target_->GetTranslation().x + offset_.x;
	}

	BaseCamera::Update();
}

#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
void FollowCamera::Debug_Gui() {
	if (ImGui::TreeNode("FollowCamera")) {
		ImGui::Begin("Camera");

		ImGui::DragFloat3("translate", &transform_.translate.x, 0.1f);
		ImGui::DragFloat3("rotate", &transform_.rotate.x, 0.1f);

		ImGui::End();
		ImGui::TreePop();
	}
}
#endif
