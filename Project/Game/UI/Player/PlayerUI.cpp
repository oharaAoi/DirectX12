#include "PlayerUI.h"
#include "Engine/Engine.h"


PlayerUI::PlayerUI() {}
PlayerUI::~PlayerUI() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerUI::Init(Player* player) {
	objs3d_["hpGauge"] = std::make_unique<BaseGameObject>();
	objs3d_["hpGauge"]->Init();
	objs3d_["hpGauge"]->SetObject("playerHpGauge.obj");
	objs3d_["hpGauge"]->SetIsLighting(false);
	objs3d_["hpGauge"]->SetColor(Vector4(0.0f, 0.7f, 0.0f, 0.9f));
	objs3d_["hpGauge"]->GetTransform()->SetQuaternion(Quaternion::AngleAxis(-90.0f * toRadian, Vector3::UP()));

	objs3d_["hpOutSide"] = std::make_unique<BaseGameObject>();
	objs3d_["hpOutSide"]->Init();
	objs3d_["hpOutSide"]->SetObject("playerHpOutSide.obj");
	objs3d_["hpOutSide"]->SetIsLighting(false);
	objs3d_["hpOutSide"]->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.2f));

	objs3d_["hpGauge"]->GetTransform()->SetParentTranslation(player->GetTransform()->GetTranslation());
	objs3d_["hpOutSide"]->GetTransform()->SetParentTranslation(player->GetTransform()->GetTranslation());

	objs3d_["hpOutSide"]->GetTransform()->SetParentRotate(objs3d_["hpGauge"]->GetTransform()->GetQuaternion());

	objs3d_["hpGauge"]->GetTransform()->SetTranslaion(Vector3(-1.8f, -2.0f, 3.0f));
	objs3d_["hpOutSide"]->GetTransform()->SetTranslaion(Vector3(-1.8f, -2.0f, 3.0f));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerUI::Update(float hp, float hpLimit) {
	float raito = hp / hpLimit;
	objs3d_["hpGauge"]->GetTransform()->SetScale(Vector3(1.0f,1.0f, raito));
	objs3d_["hpGauge"]->Update();
	objs3d_["hpOutSide"]->Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerUI::Draw() {
	objs3d_["hpGauge"]->Draw();
	objs3d_["hpOutSide"]->Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Setter
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerUI::SetPlayerScreenPos(const Matrix4x4& playerMat, const Matrix4x4& vpvpMat) {
	Vector3 pos = Transform(Vector3::ZERO(), playerMat * vpvpMat);
	playerScreenPos_ = { pos.x, pos.y };
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
void PlayerUI::Debug_Gui() {
	if (ImGui::TreeNode("PlayerUI")) {
		ImGui::Begin("PlayerUI");
		objs3d_["hpGauge"]->Debug_Gui();
		ImGui::End();
		ImGui::TreePop();
	}
}
#endif