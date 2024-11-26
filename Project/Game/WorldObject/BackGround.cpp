#include "BackGround.h"

BackGround::BackGround() {
}

BackGround::~BackGround() {
}

void BackGround::Finalize() {
}

void BackGround::Init() {
	BaseGameObject::Init();
	SetObject("BackGround.obj");


	transform_->SetTranslaion(Vector3(0.0f, 0.0f, 30.0f));
}

void BackGround::Update() {
	BaseGameObject::Update();
}

void BackGround::Draw() const {
	BaseGameObject::Draw();
}

#ifdef _DEBUG
void BackGround::Debug_Gui() {
	ImGui::Begin("BackGround");
	transform_->Debug_Gui();
	ImGui::End();
}
#endif