#include "GameScene.h"

GameScene::GameScene() {
}

GameScene::~GameScene() {
}

void GameScene::Finalize() {
}

void GameScene::Init() {
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

	fall_ = std::make_unique<Fall>();
	fall_->Init();
	fall_->GetTransform()->SetTranslaion({ 8.0f,8.0f,0.0f });


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
	
	// -------------------------------------------------
	// ↓ 初期化時にやりたい処理を行う
	// -------------------------------------------------

	isDebugCamera_ = false;
	followCamera_->SetTarget(player_->GetTransform());

	boss_->SetEditer(bossLeftAttackEditer_.get(), bossRightAttackEditer_.get());
	boss_->SetGameScene(this);

}

void GameScene::Update() {
	// -------------------------------------------------
	// ↓ worldObjectの更新
	// -------------------------------------------------

	skydome_->Update();

	field_->Update();

	// -------------------------------------------------
	// ↓ GameObjectの更新
	// -------------------------------------------------
	boss_->SetPlayerPos(player_->GetWorldPos());
	boss_->Update();

	// playerの処理
	player_->SetInverMatrix(followCamera_->GetVPVMatrix().Inverse());
	player_->SetCameraZDis(followCamera_->GetTranslate().z);

	player_->CheckBossToLength(boss_->GetBossBodyPos());
	if (!isDebugCamera_) {
		player_->Update();
	}

	// missileの処理
	missileList_.remove_if([](auto& enemy) {return !enemy->GetIsAlive(); });
	for (auto& missile : missileList_) {
		missile->Update();
	}

	testCollisionObj_->Update();
	testCollisionObj2_->Update();
	testCollisionObj3_->Update();

	fall_->Update();

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

	// mesh
	collisionManager_->AddCollider(field_->GetMeshCollider());
	collisionManager_->AddCollider(player_->GetMeshCollider());

	collisionManager_->AddCollider(boss_->GetBossCore()->GetMeshCollider());
	collisionManager_->AddCollider(boss_->GetBossRightHand()->GetMeshCollider());
	collisionManager_->AddCollider(boss_->GetBossLeftHand()->GetMeshCollider());
	collisionManager_->AddCollider(player_->GetWireTip()->GetMeshCollider());
	
	// バリアがあったらコリジョンのリストに追加
	if (boss_->GetBossBarrier() != nullptr) {
		collisionManager_->AddCollider(boss_->GetBossBarrier()->GetMeshCollider());
	}

	for (auto& missile : missileList_) {
		collisionManager_->AddCollider(missile->GetMeshCollider());
	}

	collisionManager_->CheckAllCollision();

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

#ifdef _DEBUG
	Debug_Gui();
#endif
}

void GameScene::Draw() const {
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
	for (auto& missile : missileList_) {
		missile->Debug_Draw();
	}


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
	for (auto& missile : missileList_) {
		missile->Draw();
	}

	Engine::SetPipeline(PipelineType::NormalPipeline);
	player_->Draw();

	testCollisionObj_->Draw();
	testCollisionObj2_->Draw();
	testCollisionObj3_->Draw();

	fall_->Draw();

	boss_->PostDraw();

	// -------------------------------------------------
	// ↓ UIの描画
	// -------------------------------------------------
	
}

void GameScene::AddMissile(const Vector3& targePos, const Vector3& firePos) {
	auto& newMissile = missileList_.emplace_back(std::make_unique<Missile>());
	newMissile->Init();
	newMissile->Pop(targePos, firePos);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void GameScene::Debug_Gui() {
	ImGui::Begin("GameScene");
	{
		if (ImGui::TreeNode("Camera")) {
			ImGui::Checkbox("isDebug", &isDebugCamera_);
			
			debugCamera_->Debug_Gui();
			followCamera_->Debug_Gui();
			ImGui::TreePop();
		}

		if (Input::IsTriggerKey(DIK_C)) {
			isDebugCamera_ = !isDebugCamera_;
		}
	}

	{
		if (ImGui::TreeNode("GameObject")) {
			player_->Debug_Gui();
			boss_->Debug_Gui();
			ImGui::TreePop();
		}
	}

	{
		if (ImGui::TreeNode("WorldObject")) {
			field_->Debug_Gui();
			ImGui::TreePop();
		}
	}

	{
		if (ImGui::TreeNode("Manager")) {
			enemyManager_->Debug_Gui();
			ImGui::TreePop();
		}
	}

	{
		if (ImGui::TreeNode("AdjustmentItem")) {
			// Updateだが実質Gui表示なためここで更新
			adjustmentItem_->Update();
			ImGui::TreePop();
		}
	}

	
	boss_->CheckMouseCursolCollision(debugCamera_->GetVpvpMatrix());
	bossLeftAttackEditer_->Update();
	bossRightAttackEditer_->Update();
	{
		if (ImGui::TreeNode("BossAttackEditer")) {
			ImGui::Begin("BossAttackEditer");
			if (ImGui::TreeNode("Left")) {
				bossRightAttackEditer_->AddPoint();
				bossRightAttackEditer_->DeletePoint(debugCamera_->GetVpvpMatrix());
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Right")) {
				bossRightAttackEditer_->AddPoint();
				bossRightAttackEditer_->DeletePoint(debugCamera_->GetVpvpMatrix());
				ImGui::TreePop();
			}
			ImGui::End();
			ImGui::TreePop();
		}
	}

	if (ImGui::Button("Fire")) {
		AddMissile(player_->GetWorldPos(), boss_->GetBossBodyPos());
	}

	ImGui::End();
}

#endif