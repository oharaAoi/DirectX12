#include "ResultScene.h"

ResultScene::ResultScene() {
}

ResultScene::~ResultScene() {
}

void ResultScene::Init() {
	clear_ = Engine::CreateSprite("clear.png");
	clear_->SetCenterPos({ 640.0f, 360.0f });
}

void ResultScene::Update() {
	clear_->Update();

	if (Input::IsTriggerKey(DIK_SPACE)) {
		SetNextScene(SceneType::Scene_Game);
	}
}

void ResultScene::Draw() const {
	Engine::SetPipeline(PipelineType::NormalSpritePipeline);
	clear_->Draw();
}

void ResultScene::Debug_Gui() {
}
