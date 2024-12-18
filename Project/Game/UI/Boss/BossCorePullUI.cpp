#include "BossCorePullUI.h"

BossCorePullUI::BossCorePullUI() {
}

BossCorePullUI::~BossCorePullUI() {
}

void BossCorePullUI::Init() {
	objs3d_["pullGauge"] = std::make_unique<BaseGameObject>();
	objs3d_["pullGauge"]->Init();
	objs3d_["pullGauge"]->SetObject("playerHpGauge.obj");
	objs3d_["pullGauge"]->SetIsLighting(false);
	objs3d_["pullGauge"]->SetColor(Vector4(0.9f, 0.9f, 0.0f, 0.9f));
	objs3d_["pullGauge"]->GetTransform()->SetQuaternion(Quaternion::AngleAxis(-90.0f * toRadian, Vector3::UP()));

	objs3d_["pullOutSide"] = std::make_unique<BaseGameObject>();
	objs3d_["pullOutSide"]->Init();
	objs3d_["pullOutSide"]->SetObject("playerHpOutSide.obj");
	objs3d_["pullOutSide"]->SetIsLighting(false);
	objs3d_["pullOutSide"]->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.2f));

	pos_ = Vector3(-1.9f, 3.0f, 8.5f);

	objs3d_["pullGauge"]->GetTransform()->SetTranslaion(pos_);
	objs3d_["pullOutSide"]->GetTransform()->SetTranslaion(pos_);

	objs3d_["pullOutSide"]->GetTransform()->SetQuaternion(Quaternion::AngleAxis(-90.0f * toRadian, Vector3::UP()));
}

void BossCorePullUI::Update(float hp, float hpLimit) {
	float hpRaito = hp / hpLimit;
	hpRaito = std::clamp(hpRaito, 0.0f, 1.0f);

	objs3d_["pullGauge"]->GetTransform()->SetTranslaion(pos_);
	objs3d_["pullOutSide"]->GetTransform()->SetTranslaion(pos_);

	objs3d_["pullGauge"]->GetTransform()->SetScale(Vector3(1.0f, 1.0f, hpRaito));
	objs3d_["pullGauge"]->Update();
	objs3d_["pullOutSide"]->Update();
}

void BossCorePullUI::Draw() {
	objs3d_["pullGauge"]->Draw();
	objs3d_["pullOutSide"]->Draw();
}

#ifdef _DEBUG
void BossCorePullUI::Debug_Gui() {
	if (ImGui::TreeNode("BossCoreUI")) {
		ImGui::Begin("BossHpUI");
		ImGui::DragFloat3("uiPos", &pos_.x, 0.1f);
		objs3d_["pullGauge"]->Debug_Gui();

		ImGui::End();
		ImGui::TreePop();
	}
}
#endif