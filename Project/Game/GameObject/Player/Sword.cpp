#include "Sword.h"

Sword::Sword() {}
Sword::~Sword() {}
void Sword::Finalize() {}

void Sword::Init() {
	BaseGameObject::Init();
	SetObject("sword.obj");
	transform_->scale_ = Vector3(100, 100, 100);	// 親子関係で小さくなってしまっているため

	swordTip_ = std::make_unique<BaseGameObject>();
	swordTip_->Init();
	swordTip_->SetObject("plane.obj");
	swordTip_->GetTransform()->SetParent(transform_->GetWorldMatrix());

	swordRoot_ = std::make_unique<BaseGameObject>();
	swordRoot_->Init();
	swordRoot_->SetObject("plane.obj");
	swordRoot_->GetTransform()->SetParent(transform_->GetWorldMatrix());

}

void Sword::Update() {
	swordRotate_ = TransformNormal(Vector3::RIGHT(), transform_->GetWorldMatrix());

	swordTip_->GetTransform()->SetTranslaion(swordTipOffset_);
	swordRoot_->GetTransform()->SetTranslaion(swordRootOffset_);

	swordTip_->Update();
	swordRoot_->Update();

	BaseGameObject::Update();
}

void Sword::Draw() const {
	swordTip_->Draw();
	swordRoot_->Draw();
	BaseGameObject::Draw();
}

void Sword::Debug_Gui() {
	ImGui::DragFloat3("swordTipOffset", &swordTipOffset_.x, 0.1f);
	ImGui::DragFloat3("swordRootOffset", &swordRootOffset_.x, 0.1f);
}
