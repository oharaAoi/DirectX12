#include "TitlteUI.h"
#include "Engine.h"

TitlteUI::TitlteUI() {
}

TitlteUI::~TitlteUI() {
}

void TitlteUI::Init() {
	titleSprite_ = Engine::CreateSprite("title.png");
	titleSprite_->SetTranslate({400.0f, 300.0f});

	pushA_ = Engine::CreateSprite("pushA.png");
	pushA_->SetTranslate({ 400.0f, 550.0f});
	pushA_->SetScale({0.8f, 0.8f});
}

void TitlteUI::Update() {
	titleSprite_->Update();
	pushA_->Update();
}

void TitlteUI::Draw() const {
	titleSprite_->Draw();
	pushA_->Draw();
}
