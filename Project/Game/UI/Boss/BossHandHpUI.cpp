#include "BossHandHpUI.h"
#include "Game/GameObject/Boss/BaseBossHand.h"

BossHandHpUI::BossHandHpUI() {
}

BossHandHpUI::~BossHandHpUI() {
}

void BossHandHpUI::Init(BaseBossHand* hand) {
	objs3d_["hpGauge"] = std::make_unique<BaseGameObject>();
	objs3d_["hpGauge"]->Init();
	objs3d_["hpGauge"]->SetObject("playerHpGauge.obj");
	objs3d_["hpGauge"]->SetIsLighting(false);
	objs3d_["hpGauge"]->SetColor(Vector4(0.9f, 0.0f, 0.0f, 0.9f));
	objs3d_["hpGauge"]->GetTransform()->SetQuaternion(Quaternion::AngleAxis(-90.0f * toRadian, Vector3::UP()));

	objs3d_["hpOutSide"] = std::make_unique<BaseGameObject>();
	objs3d_["hpOutSide"]->Init();
	objs3d_["hpOutSide"]->SetObject("playerHpOutSide.obj");
	objs3d_["hpOutSide"]->SetIsLighting(false);
	objs3d_["hpOutSide"]->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.2f));

	objs3d_["hpGauge"]->GetTransform()->SetParentTranslation(hand->GetTransform()->GetTranslation());
	objs3d_["hpOutSide"]->GetTransform()->SetParentTranslation(hand->GetTransform()->GetTranslation());

	objs3d_["hpOutSide"]->GetTransform()->SetParentRotate(objs3d_["hpGauge"]->GetTransform()->GetQuaternion());

	objs3d_["hpGauge"]->GetTransform()->SetTranslaion(Vector3(-2.5f, -3.5f, 18.0f));
	objs3d_["hpOutSide"]->GetTransform()->SetTranslaion(Vector3(-2.5f, -3.5f, 18.0f));
}

void BossHandHpUI::Update(float hp, float hpLimit) {
	float raito = hp / hpLimit;
	objs3d_["hpGauge"]->GetTransform()->SetScale(Vector3(1.0f, 1.0f, raito));
	objs3d_["hpGauge"]->Update();
	objs3d_["hpOutSide"]->Update();
}

void BossHandHpUI::Draw() {
	objs3d_["hpGauge"]->Draw();
	objs3d_["hpOutSide"]->Draw();
}

#ifdef _DEBUG
void BossHandHpUI::Debug_Gui() {
	if (ImGui::TreeNode("BossHandHpUI")) {
		ImGui::Begin("BossHandHpUI");
		objs3d_["hpGauge"]->Debug_Gui();
		ImGui::End();
		ImGui::TreePop();
	}
}
#endif