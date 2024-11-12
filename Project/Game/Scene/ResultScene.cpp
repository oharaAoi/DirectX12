#include "ResultScene.h"

ResultScene::ResultScene() {
}

ResultScene::~ResultScene() {
}

void ResultScene::Init() {
	clear_ = Engine::CreateSprite("clear.png");
	clear_->SetCenterPos({ 640.0f, 360.0f });

	AudioPlayer::SinglShotPlay("fanfare.wav", 0.5f);
}

void ResultScene::Update() {
	clear_->Update();

	if (Input::IsTriggerKey(DIK_SPACE)) {
		SetNextScene(SceneType::Scene_Game);
		AudioPlayer::SinglShotPlay("push.mp3", 0.3f);
	}
}

void ResultScene::Draw() const {
	Engine::SetPipeline(PipelineType::NormalSpritePipeline);
	clear_->Draw();
}
#ifdef  _DEBUG
void ResultScene::Debug_Gui() {
}

#endif //  _DEBUG
