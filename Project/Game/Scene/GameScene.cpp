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
	// ↓ system
	// -------------------------------------------------
	lockOn_ = std::make_unique<LockOn>();
	lockOn_->Init();

	// -------------------------------------------------
	// ↓ UI
	// -------------------------------------------------
	score_ = std::make_unique<Score>();
	score_->Init();

	controlUI_ = std::make_unique<ControlUI>();
	controlUI_->Init();

	trail_ = std::make_unique<Trail>();
	trail_->Init();

	panel_ = std::make_unique<FadePanel>();
	panel_->Init();
	panel_->SetFadeOutOpen(1.0f);

	// -------------------------------------------------
	// ↓ Manager
	// -------------------------------------------------
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->Init();

	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Init();

	// -------------------------------------------------
	// ↓ その他設定
	// -------------------------------------------------
	player_->SetFollowCamera(followCamera_.get());
	player_->SetLockOn(lockOn_.get());
	player_->SetTrail(trail_.get());

	followCamera_->SetTarget(player_->GetTransform());
	followCamera_->SetLockOn(lockOn_.get());

	lockOn_->SetEnemyManger(enemyManager_.get());

#ifdef _DEBUG
	
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
		Render::SetViewProjection2D(followCamera_->GetViewMatrix2D(), followCamera_->GetProjectionMatrix2D());
	}
	

	// -------------------------------------------------
	// ↓ world,GameObjectの更新
	// -------------------------------------------------

	ground_->Update();
	skydome_->Update();

	player_->Update();

	trail_->Update();

	lockOn_->SetCameraMat(followCamera_->GetCameraMatrix());
	lockOn_->Update();

	enemyManager_->SetPlayerPos(player_->GetTransform()->GetTranslation());
	enemyManager_->Update();

	// -------------------------------------------------
	// ↓ Collision判定
	// -------------------------------------------------
	collisionManager_->Reset();

	collisionManager_->AddCollider(player_->GetCollider());

	if (player_->GetIsAttack()) {
		collisionManager_->AddCollider(player_->GetAttackCollider());
	}

	for (auto& enemy : enemyManager_->GetNormalEnemyList()) {
		collisionManager_->AddCollider(enemy->GetCollider());
	}

	collisionManager_->CheckAllCollision();

	// -------------------------------------------------
	// ↓ UIの更新
	// -------------------------------------------------
	score_->Update(enemyManager_->GetDownNum());

	controlUI_->Update();

	panel_->Update();
}

void GameScene::Draw() const {
#ifdef _DEBUG
	
	player_->Debug_Draw();

	enemyManager_->Debug_Draw();
#endif
	Engine::SetPipeline(PipelineType::NormalPipeline);
	skydome_->Draw();
	ground_->Draw();

	player_->Draw();

	enemyManager_->Draw();

	Engine::SetPipeline(PipelineType::TrailPipeline);
	trail_->Draw(Render::GetViewProjection());

	Engine::SetPipeline(PipelineType::NormalPipeline);
	Engine::ClearDepth();
	lockOn_->Draw();

	Engine::SetPipeline(PipelineType::SpriteNormalBlendPipeline);
	score_->Draw();
	controlUI_->Draw();

	panel_->Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void GameScene::Debug_Gui() {
	ImGui::Checkbox("debugMode", &isDebug_);
}
#endif