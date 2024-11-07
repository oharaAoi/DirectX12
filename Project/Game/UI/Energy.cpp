#include "Energy.h"

Energy::Energy() {
}

Energy::~Energy() {
}

void Energy::Init() {
	outside_ = Engine::CreateSprite("razer_outside.png");
	energy_ = Engine::CreateSprite("energy.png");

	centerPos_ = { 130.0f, 460.0f };

	outside_->SetCenterPos(centerPos_);
	energy_->SetCenterPos(centerPos_);
}

void Energy::Update(float energy) {
	energy_->SetUvMinSize(Vector2(0.0f, 1.0f - energy));

	outside_->SetCenterPos(centerPos_);
	energy_->SetCenterPos(centerPos_);

	outside_->Update();
	energy_->Update();
}

void Energy::Draw() const {
	outside_->Draw();
	energy_->Draw();
}

#ifdef _DEBUG
void Energy::Debug_Gui() {
	if (ImGui::TreeNode("Energy")) {
		ImGui::Begin("Energy");
		ImGui::DragFloat2("centerPos", &centerPos_.x, 1.0f);
		ImGui::End();
		ImGui::TreePop();
	}
}
#endif