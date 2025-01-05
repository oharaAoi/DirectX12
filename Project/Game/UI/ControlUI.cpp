#include "ControlUI.h"
#include "Engine/Engine.h"
#include "Engine/Editer/Window/EditerWindows.h"

ControlUI::ControlUI() {
}

ControlUI::~ControlUI() {
}

void ControlUI::Init() {

	for (uint32_t oi = 0; oi < 2; ++oi) {
		if (oi == 0) {
			controlUI_[oi] = Engine::CreateSprite("controlUI1.png");
			controlUI_[oi]->SetScale({ 0.8f, 0.8f });
		} else {
			controlUI_[oi] = Engine::CreateSprite("controlUI2.png");
		}

		controlUI_[oi]->SetTranslate(controlUIPos_[oi]);
	}
	
#ifdef _DEBUG
	EditerWindows::AddObjectWindow(std::bind(&ControlUI::Debug_Gui, this), "controlUI");
#endif
}

void ControlUI::Update() {
	for (uint32_t oi = 0; oi < 2; ++oi) {
		controlUI_[oi]->SetTranslate(controlUIPos_[oi]);
		controlUI_[oi]->Update();
	}
}

void ControlUI::Draw() const {
	for (uint32_t oi = 0; oi < 2; ++oi) {
		controlUI_[oi]->Draw();
	}
}

#ifdef _DEBUG
void ControlUI::Debug_Gui() {
	ImGui::DragFloat2("uiPos_1", &controlUIPos_[0].x, 1.0f);
	ImGui::DragFloat2("uiPos_2", &controlUIPos_[1].x, 1.0f);
}
#endif