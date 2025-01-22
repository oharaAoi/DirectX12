#include "TitleScene.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Editer/Window/EditerWindows.h"

TitleScene::TitleScene() {}
TitleScene::~TitleScene() { Finalize(); }
void TitleScene::Finalize() {
	bgm_->Finalize();
}

void TitleScene::Init() {
	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	adjust->Init("TitleScene");

	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Init();
	camera_ = std::make_unique<Camera3d>();
	camera2d_ = std::make_unique<Camera2d>();
	camera_->Init();
	camera2d_->Init();

	// -------------------------------------------------
	// ↓ worldObject
	// -------------------------------------------------
	ground_ = std::make_unique<Ground>();
	ground_->Init();

	skydome_ = std::make_unique<Skydome>();
	skydome_->Init();

	// -------------------------------------------------
	// ↓ UI
	// -------------------------------------------------

	titleUI_ = std::make_unique<TitlteUI>();
	titleUI_->Init();

	panel_ = std::make_unique<FadePanel>();
	panel_->Init();

	bgm_ = std::make_unique<AudioPlayer>();
	bgm_->Init("titleBGM.mp3");
	bgm_->Play(true, 0.2f);

	isNexScene_ = false;

}

void TitleScene::Update() {
	// -------------------------------------------------
	// ↓ cameraの更新
	// -------------------------------------------------

	camera_->Update();
	camera2d_->Update();

	if (isDebug_) {
		debugCamera_->Update();
	} 

	// -------------------------------------------------
	// ↓ sceneの更新
	// -------------------------------------------------

	if (Input::GetIsPadTrigger(BUTTON_A)) {
		isNexScene_ = true;
		panel_->SetFadeOut(0.8f);
		AudioPlayer::SinglShotPlay("pushA.mp3", 0.6f);
		bgm_->Stop();
	}

	if (isNexScene_) {
		if (panel_->GetIsFinished()) {
			SetNextSceneType(SceneType::GAME);
		}
	}

	// -------------------------------------------------
	// ↓ world,GameObjectの更新
	// -------------------------------------------------

	ground_->Update();
	skydome_->Update();

	titleUI_->Update();
	panel_->Update();
}

void TitleScene::Draw() const {
	Engine::SetPipeline(PipelineType::NormalPipeline);
	skydome_->Draw();
	ground_->Draw();

	Engine::ClearDepth();
	Engine::SetPipeline(PipelineType::SpriteNormalBlendPipeline);
	titleUI_->Draw();

	panel_->Draw();
}

#ifdef _DEBUG
void TitleScene::Debug_Gui() {
	ImGui::Checkbox("debugMode", &isDebug_);
}
#endif