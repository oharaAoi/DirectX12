#include "GameScene.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Editer/Window/EditerWindows.h"

GameScene::GameScene() {
}

GameScene::~GameScene() {
}


void GameScene::Finalize() {
}

void GameScene::Init() {
	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	adjust->Init("GameScene");

	debugCamera_ = std::make_unique<DebugCamera>();
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Init();

	// -------------------------------------------------
	// ↓ worldObject
	// -------------------------------------------------
	ground_ = std::make_unique<Ground>();
	ground_->Init();

	skydome_ = std::make_unique<Skydome>();
	skydome_->Init();

	// -------------------------------------------------
	// ↓ gameObject
	// -------------------------------------------------
	player_ = std::make_unique<Player>();
	player_->Init();

	// -------------------------------------------------
	// ↓ Manager
	// -------------------------------------------------
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->Init();

	// -------------------------------------------------
	// ↓ その他設定
	// -------------------------------------------------
	player_->SetFollowCamera(followCamera_.get());

	followCamera_->SetTarget(player_->GetTransform());

#ifdef _DEBUG
	EditerWindows::AddObjectWindow(std::bind(&GameScene::Debug_Gui, this), "Scene");
#endif // _DEBUG
}

void GameScene::Update() {
	// -------------------------------------------------
	// ↓ cameraの更新
	// -------------------------------------------------

	debugCamera_->Update();
	followCamera_->Update();

	if (isDebug_) {
		Render::SetEyePos(debugCamera_->GetWorldTranslate());
		Render::SetViewProjection(debugCamera_->GetViewMatrix(), debugCamera_->GetProjectionMatrix());
	} else {
		Render::SetEyePos(followCamera_->GetWorldTranslate());
		Render::SetViewProjection(followCamera_->GetViewMatrix(), followCamera_->GetProjectionMatrix());
	}
	

	// -------------------------------------------------
	// ↓ world,GameObjectの更新
	// -------------------------------------------------

	ground_->Update();
	skydome_->Update();

	player_->Update();

	enemyManager_->Update();
}

void GameScene::Draw() const {
	Engine::SetPipeline(PipelineType::NormalPipeline);
	skydome_->Draw();
	ground_->Draw();

	player_->Draw();

	enemyManager_->Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void GameScene::Debug_Gui() {
	ImGui::Checkbox("debugMode", &isDebug_);
}
#endif