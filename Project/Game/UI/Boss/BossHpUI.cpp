#include "BossHpUI.h"

BossHpUI::BossHpUI(Boss* pBoss) {
	pBoss_ = pBoss;
}

BossHpUI::~BossHpUI() {
}

void BossHpUI::Init() {
	obj3d_["hpGauge"] = std::make_unique<BaseGameObject>();
	obj3d_["hpGauge"]->Init();
	obj3d_["hpGauge"]->SetObject("bossGauge.obj");

	obj3d_["hpOutSide"] = std::make_unique<BaseGameObject>();
	obj3d_["hpOutSide"]->Init();
	obj3d_["hpOutSide"]->SetObject("bossHpOutSide.obj");

	obj3d_["hpGauge"]->GetTransform()->SetQuaternion(Quaternion::AngleAxis(-90.0f * toRadian, Vector3::UP()));
	obj3d_["hpGauge"]->GetTransform()->SetTranslaion(Vector3(-6.0f, 12.0f, 17.0f));

	obj3d_["hpOutSide"]->SetIsLighting(false);
	obj3d_["hpGauge"]->SetIsLighting(false);
	obj3d_["hpGauge"]->SetColor(Vector4(1.0f, 0.0f, 0.0f, 0.8f));
	obj3d_["hpOutSide"]->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.2f));
	obj3d_["hpOutSide"]->GetTransform()->SetParentTranslation(obj3d_["hpGauge"]->GetTransform()->GetTranslation());
	obj3d_["hpOutSide"]->GetTransform()->SetParentRotate(obj3d_["hpGauge"]->GetTransform()->GetQuaternion());

	obj3d_["hpGaugeOrigin"] = std::make_unique<BaseGameObject>();
	obj3d_["hpGaugeOrigin"]->Init();
	obj3d_["hpGaugeOrigin"]->SetObject("bossGaugeOrigin.obj");

	obj3d_["hpOutSideOrigin"] = std::make_unique<BaseGameObject>();
	obj3d_["hpOutSideOrigin"]->Init();
	obj3d_["hpOutSideOrigin"]->SetObject("bossHpOutSideOrigin.obj");

	obj3d_["hpGaugeOrigin"]->SetColor(Vector4(1.0f, 0.0f, 0.0f, 0.8f));
	obj3d_["hpOutSideOrigin"]->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.2f));

	obj3d_["hpGaugeOrigin"]->GetTransform()->SetQuaternion(Quaternion::AngleAxis(-90.0f * toRadian, Vector3::UP()));
	obj3d_["hpOutSideOrigin"]->GetTransform()->SetQuaternion(Quaternion::AngleAxis(-90.0f * toRadian, Vector3::UP()));

	obj3d_["hpGaugeOrigin"]->GetTransform()->SetTranslaion(Vector3(0.0f, 12.0f, 17.0f));
	obj3d_["hpOutSideOrigin"]->GetTransform()->SetTranslaion(Vector3(0.0f, 12.0f, 17.0f));

	obj3d_["hpGaugeOrigin"]->SetIsLighting(false);
	obj3d_["hpOutSideOrigin"]->SetIsLighting(false);


	// -------------------------------------------------
	// ↓ 変数の定義
	// -------------------------------------------------
	isScaleUpBossHp_ = true;
	scaleUpTime_ = 0.0f;
}

void BossHpUI::Update(float bossHp) {
	ScaleUpBossHp();

	if (!isScaleUpBossHp_) {
		float hpRaito = bossHp / 100.0f;
		obj3d_["hpGauge"]->GetTransform()->SetScale(Vector3(1.0f, 1.0f, hpRaito));
	} else {
		obj3d_["hpGaugeOrigin"]->Update();
		obj3d_["hpOutSideOrigin"]->Update();
	}
	obj3d_["hpGauge"]->Update();
	obj3d_["hpOutSide"]->Update();
}

void BossHpUI::Draw() {
	if (!isScaleUpBossHp_) {
		obj3d_["hpGauge"]->Draw();
		obj3d_["hpOutSide"]->Draw();
	} else {
		obj3d_["hpGaugeOrigin"]->Draw();
		obj3d_["hpOutSideOrigin"]->Draw();
	}
}

void BossHpUI::ScaleUpBossHp() {
	if (isScaleUpBossHp_) {
		scaleUpTime_ += GameTimer::DeltaTime();
		float t = scaleUpTime_ / 1.0f;
		float lerpT = Ease::Out::Quint(t);
		obj3d_["hpGaugeOrigin"]->GetTransform()->SetScale(Vector3(lerpT, lerpT, lerpT));
		obj3d_["hpOutSideOrigin"]->GetTransform()->SetScale(Vector3(lerpT, lerpT, lerpT));

		if (t >= 1.0f) {
			isScaleUpBossHp_ = false;
		}
	}
}


#ifdef _DEBUG
void BossHpUI::Debug_Gui() {
	if (ImGui::TreeNode("BossHpUI")) {
		ImGui::Begin("BossHpUI");
		obj3d_["hpGaugeOrigin"]->Debug_Gui();

		ImGui::End();
		ImGui::TreePop();
	}
}
#endif