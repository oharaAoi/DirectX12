#include "Ground.h"

Ground::Ground() {
}

Ground::~Ground() {
}

void Ground::Finalize() {
}

void Ground::Init() {
	BaseGameObject::Init();
	SetObject("ground.obj");
}

void Ground::Update() {
	BaseGameObject::Update();
}

void Ground::Draw() const {
	BaseGameObject::Draw();
}

#ifdef _DEBUG
void Ground::Debug_Gui() {
	if (ImGui::TreeNode("Ground")) {
		transform_->Debug_Gui();
		ImGui::TreePop();
	}
}
#endif