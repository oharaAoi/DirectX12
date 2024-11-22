#include "BossUI.h"
#include "Engine/Engine.h"
#include "Engine/Utilities/AdjustmentItem.h"
#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
#endif

BossUI::BossUI() {}
BossUI::~BossUI() {}

void BossUI::Init() {
	sprites_["bossHp"] = Engine::CreateSprite("kari_bossHp.png");
}

void BossUI::Update() {
	sprites_["bossHp"]->Update();
}

void BossUI::Draw() {
	sprites_["bossHp"]->Draw();
}

void BossUI::AdaptAdjustment() {

}

#ifdef _DEBUG
void BossUI::Debug_Gui() {
	if (ImGui::TreeNode("BossUI")) {
		sprites_["bossHp"]->Debug_Gui("bossHp");
		ImGui::TreePop();
	}
}
#endif