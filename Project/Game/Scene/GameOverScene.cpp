#include "GameOverScene.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Editer/Window/EditerWindows.h"
#include "Engine/System/Audio/AudioPlayer.h"


GameOverScene::GameOverScene() {
}

GameOverScene::~GameOverScene() {
}

void GameOverScene::Finalize() {
}

void GameOverScene::Init() {
	debugCamera_ = std::make_unique<DebugCamera>();
	camera_ = std::make_unique<Camera>();
	camera_->Init();

	clearUI_ = Engine::CreateSprite("gameOverBack.png");
	clearUI_->SetTranslate({ 640.0f, 360.0f });

	clearBack_ = Engine::CreateSprite("GameOverUI.png");
	clearBack_->SetTranslate({ 640.0f, 360.0f });

	AudioPlayer::SinglShotPlay("gameOver.mp3", 0.6f);
}

void GameOverScene::Update() {
	// -------------------------------------------------
	// ↓ cameraの更新
	// -------------------------------------------------

	debugCamera_->Update();
	camera_->Update();

	if (isDebug_) {
		Render::SetEyePos(debugCamera_->GetWorldTranslate());
		Render::SetViewProjection(debugCamera_->GetViewMatrix(), debugCamera_->GetProjectionMatrix());
	} else {
		Render::SetEyePos(camera_->GetWorldTranslate());
		Render::SetViewProjection(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());

		Render::SetViewProjection2D(camera_->GetViewMatrix2D(), camera_->GetProjectionMatrix2D());
	}

	// -------------------------------------------------
	// ↓ sceneの更新
	// -------------------------------------------------

	if (Input::GetIsPadTrigger(BUTTON_A)) {
		AudioPlayer::SinglShotPlay("pushA.mp3", 0.6f);
		SetNextSceneType(SceneType::TITLE);
	}

	clearBack_->Update();
	clearUI_->Update();
}

void GameOverScene::Draw() const {
	Engine::SetPipeline(PipelineType::SpriteNormalBlendPipeline);
	clearUI_->Draw();
	clearBack_->Draw();
}

#ifdef _DEBUG
void GameOverScene::Debug_Gui() {
}
#endif