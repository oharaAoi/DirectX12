#include "GameScene.h"
#include "Engine/Utilities/AdjustmentItem.h"

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

	ground_ = std::make_unique<Ground>();
	ground_->Init();

	skydome_ = std::make_unique<Skydome>();
	skydome_->Init();

	player_ = std::make_unique<Player>();
	player_->Init();

	player_->SetFollowCamera(followCamera_.get());

	followCamera_->SetTarget(player_->GetTransform());
}

void GameScene::Update() {
	// -------------------------------------------------
	// ↓ カメラの更新
	// -------------------------------------------------

	debugCamera_->Update();
	followCamera_->Update();
	Render::SetEyePos(followCamera_->GetWorldTranslate());
	Render::SetViewProjection(followCamera_->GetViewMatrix(), followCamera_->GetProjectionMatrix());

	// -------------------------------------------------
	// ↓ GameObjectの更新
	// -------------------------------------------------

	ground_->Update();
	skydome_->Update();

	player_->Update();
}

void GameScene::Draw() const {
	Engine::SetPipeline(PipelineType::NormalPipeline);
	skydome_->Draw();
	ground_->Draw();

	player_->Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void GameScene::Debug_Gui() {
	
}

#endif