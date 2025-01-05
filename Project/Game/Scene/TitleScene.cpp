#include "TitleScene.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Editer/Window/EditerWindows.h"

TitleScene::TitleScene() {}
TitleScene::~TitleScene() {}
void TitleScene::Finalize() {}

void TitleScene::Init() {
	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	adjust->Init("TitleScene");

	debugCamera_ = std::make_unique<DebugCamera>();
	camera_ = std::make_unique<Camera>();
	camera_->Init();

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

}

void TitleScene::Update() {
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
	}

	// -------------------------------------------------
	// ↓ world,GameObjectの更新
	// -------------------------------------------------

	ground_->Update();
	skydome_->Update();

	titleUI_->Update();
}

void TitleScene::Draw() const {
	Engine::SetPipeline(PipelineType::NormalPipeline);
	skydome_->Draw();
	ground_->Draw();

	Engine::SetPipeline(PipelineType::SpriteNormalBlendPipeline);
	titleUI_->Draw();
}

#ifdef _DEBUG
void TitleScene::Debug_Gui() {
}
#endif