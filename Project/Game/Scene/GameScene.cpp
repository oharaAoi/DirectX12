#include "GameScene.h"

GameScene::GameScene() {
}

GameScene::~GameScene() {
}

void GameScene::Init() {
<<<<<<< HEAD
	adjustmentItem_ = AdjustmentItem::GetInstance();
	adjustmentItem_->Init("gameScene");

	// -------------------------------------------------
	// ↓ Cameraの初期化
	// -------------------------------------------------

	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Init();

	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Init();

	// -------------------------------------------------
	// ↓ worldObjectの初期化
	// -------------------------------------------------
	skydome_ = std::make_unique<Skydome>();
	skydome_->Init();

	field_ = std::make_unique<Field>();
	field_->Init();

	// -------------------------------------------------
	// ↓ GameObjectの初期化
	// -------------------------------------------------
	player_ = std::make_unique<Player>();
	player_->Init();

	boss_ = std::make_unique<Boss>();
	boss_->Init();

	testCollisionObj_ = std::make_unique<TestCollisionObj>();
	testCollisionObj_->Init();
	testCollisionObj_->SetPlayer(player_.get());

	testCollisionObj2_ = std::make_unique<TestCollisionObj>();
	testCollisionObj2_->Init();
	testCollisionObj2_->SetPlayer(player_.get());
	testCollisionObj2_->SetTag("canCatchObj");
	testCollisionObj2_->GetTransform()->SetTranslaion({ -3.0f,4.0f,0.0f });


	testCollisionObj3_ = std::make_unique<TestCollisionObj>();
	testCollisionObj3_->Init();
	testCollisionObj3_->SetPlayer(player_.get());
	testCollisionObj3_->SetTag("canPullObj");
	testCollisionObj3_->GetTransform()->SetTranslaion({ -6.0f,0.5f,0.0f });


	// -------------------------------------------------
	// ↓ Managerの初期化
	// -------------------------------------------------
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->Init();

	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Init();

	// -------------------------------------------------
	// ↓ Editer初期化
	// -------------------------------------------------
	bossLeftAttackEditer_ = std::make_unique<BossAttackEditer>();
	bossLeftAttackEditer_->Init();

	bossRightAttackEditer_ = std::make_unique<BossAttackEditer>();
	bossRightAttackEditer_->Init();

	// -------------------------------------------------
	// ↓ UI初期化
	// -------------------------------------------------
=======
>>>>>>> main
	
}

void GameScene::Update() {
<<<<<<< HEAD
	// -------------------------------------------------
	// ↓ worldObjectの更新
	// -------------------------------------------------

	skydome_->Update();

	field_->Update();

	// -------------------------------------------------
	// ↓ GameObjectの更新
	// -------------------------------------------------
	boss_->Update();

	if (!isDebugCamera_) {
		player_->SetInverMatrix(followCamera_->GetVPVMatrix().Inverse());
		player_->SetCameraZDis(followCamera_->GetTranslate().z);
		player_->Update();
	}

	testCollisionObj_->Update();
	testCollisionObj2_->Update();
	testCollisionObj3_->Update();

	// -------------------------------------------------
	// ↓ UIの更新
	// -------------------------------------------------
	

	// -------------------------------------------------
	// ↓ Managerの更新
	// -------------------------------------------------
	
	collisionManager_->Reset();
	collisionManager_->AddCollider(player_->GetWireTipCollider());
	collisionManager_->AddCollider(testCollisionObj_.get());
	collisionManager_->AddCollider(testCollisionObj2_.get());
	collisionManager_->AddCollider(testCollisionObj3_.get());

	collisionManager_->AddCollider(player_->GetMeshCollider());
	collisionManager_->AddCollider(boss_->GetBossCore()->GetMeshCollider());
	collisionManager_->AddCollider(player_->GetWireTip()->GetMeshCollider());
	

	collisionManager_->CheckAllCollision();


	// -------------------------------------------------
	// ↓ 死亡の確認
	// -------------------------------------------------
	

	// -------------------------------------------------
	// ↓ Cameraの更新
	// -------------------------------------------------

	debugCamera_->Update();
	followCamera_->Update();

	if (!isDebugCamera_) {
		Render::SetEyePos(followCamera_->GetWorldTranslate());
		Render::SetViewProjection(followCamera_->GetViewMatrix(), followCamera_->GetProjectionMatrix());
		Render::SetViewProjection2D(followCamera_->GetViewMatrix2D(), followCamera_->GetProjectionMatrix2D());
	}
	else {
		Render::SetEyePos(debugCamera_->GetWorldTranslate());
		Render::SetViewProjection(debugCamera_->GetViewMatrix(), debugCamera_->GetProjectionMatrix());
		Render::SetViewProjection2D(debugCamera_->GetViewMatrix2D(), debugCamera_->GetProjectionMatrix2D());
	}


	// -------------------------------------------------
	// ↓ Renderの更新
	// -------------------------------------------------
	if (!isDebugCamera_) {
		eyePos_ = followCamera_->GetWorldTranslate();
		viewMat_ = followCamera_->GetViewMatrix();
		projectionMat_ = followCamera_->GetProjectionMatrix();
	} else {
		eyePos_ = debugCamera_->GetWorldTranslate();
		viewMat_ = debugCamera_->GetViewMatrix();
		projectionMat_ = debugCamera_->GetProjectionMatrix();
	}

	Render::SetEyePos(eyePos_);
	Render::SetViewProjection(viewMat_, projectionMat_);

=======
	
>>>>>>> main
#ifdef _DEBUG
	Debug_Gui();
#endif
}

void GameScene::Draw() const {
<<<<<<< HEAD
	// -------------------------------------------------
	// ↓ Debugの描画
	// -------------------------------------------------
#ifdef _DEBUG
	Engine::SetPipeline(PipelineType::PrimitivePipeline);
	DrawGrid(viewMat_, projectionMat_);

	bossLeftAttackEditer_->Draw();
	bossRightAttackEditer_->Draw();

	player_->Debug_Draw();
	boss_->Debug_Draw();

#endif
	
	// -------------------------------------------------
	// ↓ worldObjectの描画
	// -------------------------------------------------
	Engine::SetPipeline(PipelineType::NormalPipeline);
	skydome_->Draw();

	field_->Draw();

	// -------------------------------------------------
	// ↓ GameObjectの描画
	// -------------------------------------------------
	boss_->Draw();

	Engine::SetPipeline(PipelineType::NormalPipeline);
	player_->Draw();

	testCollisionObj_->Draw();
	testCollisionObj2_->Draw();
	testCollisionObj3_->Draw();

	// -------------------------------------------------
	// ↓ UIの描画
	// -------------------------------------------------
=======
>>>>>>> main
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void GameScene::Debug_Gui() {
	ImGui::Begin("GameScene");
	
	ImGui::End();
}

#endif