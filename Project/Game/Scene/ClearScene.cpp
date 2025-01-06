#include "ClearScene.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Editer/Window/EditerWindows.h"

ClearScene::ClearScene() {
}

ClearScene::~ClearScene() {
}

void ClearScene::Finalize() {
}

void ClearScene::Init() {
	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	adjust->Init("ClearScene");

	debugCamera_ = std::make_unique<DebugCamera>();
	camera_ = std::make_unique<Camera>();
	camera_->Init();

	clearUI_ = Engine::CreateSprite("clearBack.png");
	clearUI_->SetTranslate({640.0f, 360.0f});

	clearBack_ = Engine::CreateSprite("clearUI.png");
	clearBack_->SetTranslate({ 640.0f, 360.0f });
}

void ClearScene::Update() {
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
	// ↓ sceneの更新
	// -------------------------------------------------

	if (Input::GetIsPadTrigger(BUTTON_A)) {
		SetNextSceneType(SceneType::TITLE);
	}

	clearBack_->Update();
	clearUI_->Update();
}

void ClearScene::Draw() const {
	Engine::SetPipeline(PipelineType::SpriteNormalBlendPipeline);
	clearUI_->Draw();
	clearBack_->Draw();
}

#ifdef _DEBUG
void ClearScene::Debug_Gui() {
}
#endif