#include "PlayerUI.h"
#include "Engine/Engine.h"
#include "Engine/Editer/Window/EditerWindows.h"

PlayerUI::PlayerUI() {
}

PlayerUI::~PlayerUI() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerUI::Init() {
	firstHpPos_ = { 80.0f, 50.0f };
	interval_ = 80.0f;

	for (uint32_t oi = 0; oi < kMaxPlayerHp_; ++oi) {
		hpes_[oi] = Engine::CreateSprite("hart.png");
		hpes_[oi]->SetTranslate({ firstHpPos_.x + (interval_ * static_cast<float>(oi)) , firstHpPos_.y });

	}
#ifdef _DEBUG
	EditerWindows::AddObjectWindow(std::bind(&PlayerUI::Debug_Gui, this), "PlayerUI");
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerUI::Update() {
	for (uint32_t oi = 0; oi < kMaxPlayerHp_; ++oi) {
		hpes_[oi]->SetTranslate({ firstHpPos_.x + (interval_ * static_cast<float>(oi)) , firstHpPos_.y });
		hpes_[oi]->Update();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerUI::Draw() const {
	for (uint32_t oi = 0; oi < kMaxPlayerHp_; ++oi) {
		if (pPlayer_->GetHp() >= oi + 1) {
			hpes_[oi]->Draw();
		}
	}
}

#ifdef _DEBUG
void PlayerUI::Debug_Gui() {
	ImGui::DragFloat2("pos", &firstHpPos_.x, 1.0f);
}
#endif