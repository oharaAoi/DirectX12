#include "TitlteUI.h"
#include "Engine.h"

TitlteUI::TitlteUI() {
}

TitlteUI::~TitlteUI() {
}

void TitlteUI::Init() {
	titleSprite_ = Engine::CreateSprite("title.png");
	titleSprite_->SetTranslate({400.0f, 400.0f});
}

void TitlteUI::Update() {
	titleSprite_->Update();
}

void TitlteUI::Draw() const {
	titleSprite_->Draw();
}
