#include "GameScene.h"

GameScene::GameScene() {
}

GameScene::~GameScene() {
}

void GameScene::Init() {
	// -------------------------------------------------
	// ↓ Cameraの初期化
	// -------------------------------------------------
	mainCamera_ = std::make_unique<RailCamera>();
	mainCamera_->Init();

	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Init();

	// -------------------------------------------------
	// ↓ worldObjectの初期化
	// -------------------------------------------------
	skydome_ = std::make_unique<Skydome>();
	skydome_->Init();

	worldObjcts_ = std::make_unique<WorldObjects>();
	worldObjcts_->Init();

	// -------------------------------------------------
	// ↓ GameObjectの初期化
	// -------------------------------------------------
	player_ = std::make_unique<Player>();
	player_->Init();

	// 弾
	for (uint32_t index = 0; index < kBulletNum_; ++index) {
		playerBullets_[index] = std::make_unique<PlayerBullet>();
		playerBullets_[index]->Init();

		//playerBullets_[index]->GetTransform()->SetParent(player_->GetTransform()->GetWorldMatrix());
		
		if (index == 0) {
			playerBullets_[index]->SetOffset(Vector3(0.5f * -1, -0.35f, 1.3f));
		} else {
			playerBullets_[index]->SetOffset(Vector3(0.5f, -0.35f, 1.3f));
		}
	}

	// -------------------------------------------------
	// ↓ Editer初期化
	// -------------------------------------------------
	railPointEditer_ = std::make_unique<RailPointEditer>();
	railPointEditer_->InitLoad();
	railPointEditer_->Init();
	railPointEditer_->SetGameScene(this);

	/*for (size_t index = 0; index < railPointEditer_->GetRailNum(); ++index) {
		auto& newRail = rails_.emplace_back(std::make_unique<Rail>());
		newRail->Init();
	}*/

	rail_ = std::make_unique<Rail>();
	rail_->Init("./Game/Resources/", "rail.obj", (uint32_t)railPointEditer_->GetPointsSize());

	// -------------------------------------------------
	// ↓ Managerの初期化
	// -------------------------------------------------
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->Init();

	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Init();

	// -------------------------------------------------
	// ↓ UI初期化
	// -------------------------------------------------
	reticle_ = std::make_unique<Reticle>();
	reticle_->Init();

	knockDownEnemy_ = std::make_unique<KnockDownEnemy>();
	knockDownEnemy_->Init();

	totalScore_ = std::make_unique<TotalScore>();
	totalScore_->Init();

	energyUI_ = std::make_unique<Energy>();
	energyUI_->Init();

	// -------------------------------------------------
	// ↓ 初期化時にやりたい処理を行う
	// -------------------------------------------------
	player_->GetTransform()->SetParent((mainCamera_->GetCameraMatrix()));
	player_->SetPGameScene(this);

	railPointEditer_->Update();
	mainCamera_->SetControlPoints(railPointEditer_->GetRailPoints());
	
	isDebugCamera_ = false;
}

void GameScene::Update() {
	// -------------------------------------------------
	// ↓ Cameraの更新
	// -------------------------------------------------
	railPointEditer_->Update();

	mainCamera_->SetControlPoints(railPointEditer_->GetRailBasePoints());
	//mainCamera_->SetControlRotateZ(railPointEditer_->GetRailRotateZ());
	mainCamera_->Update();
	debugCamera_->Update();

	if (!isDebugCamera_) {
		Render::SetEyePos(mainCamera_->GetWorldTranslate());
		Render::SetViewProjection(mainCamera_->GetViewMatrix(), mainCamera_->GetProjectionMatrix());
		Render::SetViewProjection2D(mainCamera_->GetViewMatrix2D(), mainCamera_->GetProjectionMatrix2D());
	} else {
		Render::SetEyePos(debugCamera_->GetWorldTranslate());
		Render::SetViewProjection(debugCamera_->GetViewMatrix(), debugCamera_->GetProjectionMatrix());
		Render::SetViewProjection2D(debugCamera_->GetViewMatrix2D(), debugCamera_->GetProjectionMatrix2D());
	}

	// -------------------------------------------------
	// ↓ worldObjectの更新
	// -------------------------------------------------

	skydome_->Update();

	worldObjcts_->Update();

	for (size_t index = 0; index < (size_t)railPointEditer_->GetPointsSize(); ++index) {

		rail_->Update(railPointEditer_->GetPoints(index), railPointEditer_->GetPointsRotate(index), viewMat_, projectionMat_, (uint32_t)index);
	}

	// -------------------------------------------------
	// ↓ GameObjectの更新
	// -------------------------------------------------

	// playerの更新
	player_->SetReticlePos(ScreenToWorldCoordinate(reticle_->GetReticle2DPos(), mainCamera_->GetVPVMatrix().Inverse(), 60.0f));
	player_->Update();

	// bulletの更新
	if (player_->GetIsShot()) {
		for (auto& bullet : playerBullets_) {
			bullet->SetCameraMat(mainCamera_->GetCameraMatrix());
			bullet->GetTransform()->SetTranslaion(player_->GetWorldPos());
			bullet->GetTransform()->SetQuaternion(player_->GetShotQuaternion());
			bullet->Update();
		}
	}

	// -------------------------------------------------
	// ↓ UIの更新
	// -------------------------------------------------
	Vector3 playerPos = Transform({ 0.0, 0.0f, 0.0f }, player_->GetTransform()->GetWorldMatrix());
	reticle_->Update(playerPos, player_->GetForward(), mainCamera_->GetVpvpMatrix());

	knockDownEnemy_->SetWorldPos(ScreenToWorldCoordinate(knockDownEnemy_->GetScreenPos(), mainCamera_->GetVPVMatrix().Inverse(), 10.0f));
	knockDownEnemy_->Update();

	totalScore_->Update(player_->GetScore());

	energyUI_->Update(player_->GetShotEnergyRaito());

	// -------------------------------------------------
	// ↓ Managerの更新
	// -------------------------------------------------
	enemyManager_->SetPlayerForward(player_->GetForward());
	enemyManager_->SetPlayerPos(player_->GetWorldPos());
	enemyManager_->Update(mainCamera_->GetEyeIndex());

	collisionManager_->Reset();

	if (player_->GetIsShot()) {
		for (auto& bullet : playerBullets_) {
			collisionManager_->AddCollider(bullet->GetMeshCollider());
		}
	}

	for (auto& enemy : enemyManager_->GetList()) {
		collisionManager_->AddCollider(enemy->GetMeshCollider());
	}
	
	collisionManager_->CheckAllCollision();

	// -------------------------------------------------
	// ↓ Renderの更新
	// -------------------------------------------------
	if (!isDebugCamera_) {
		eyePos_ = mainCamera_->GetWorldTranslate();
		viewMat_ = mainCamera_->GetViewMatrix();
		projectionMat_ = mainCamera_->GetProjectionMatrix();
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
	// ↓ worldObjectの描画
	// -------------------------------------------------
	/*Engine::SetPipeline(PipelineType::PrimitivePipeline);
	railPointEditer_->Draw(viewMat_ * projectionMat_);*/

	Engine::SetPipeline(PipelineType::NormalPipeline);
#ifdef _DEBUG
	railPointEditer_->DrawObject();
#endif

	skydome_->Draw();

	worldObjcts_->Draw();

	Engine::SetPipeline(PipelineType::ParticlePipeline);
	/*for (size_t index = 0; index < rails_.size(); ++index) {
		rails_[index]->Draw();
	}*/
	rail_->Draw();

	// -------------------------------------------------
	// ↓ GameObjectの描画
	// -------------------------------------------------
	enemyManager_->Draw();

	Engine::SetPipeline(PipelineType::NormalPipeline);
	if (player_->GetIsShot()) {
		for (const auto& bullet : playerBullets_) {
			bullet->Draw();
		}
	}

	Engine::SetPipeline(PipelineType::NormalPipeline);
	player_->Draw();

	knockDownEnemy_->Draw();

	//mainCamera_->Draw();
	// -------------------------------------------------
	// ↓ UIの描画
	// -------------------------------------------------
	reticle_->Draw();

	totalScore_->Draw();

	energyUI_->Draw();

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
			mainCamera_->Debug_Gui();
			debugCamera_->Debug_Gui();
			ImGui::TreePop();
		}

		if (Input::IsTriggerKey(DIK_C)) {
			isDebugCamera_ = !isDebugCamera_;
		}
	}

	{
		if (ImGui::TreeNode("GameObject")) {
			player_->Debug_Gui();
			knockDownEnemy_->Debug_Gui();
			ImGui::TreePop();
		}
	}

	{
		if (ImGui::TreeNode("RailEditer")) {
			railPointEditer_->Debug_Gui();
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
		if (ImGui::TreeNode("UI")) {
			totalScore_->Debug_Gui();
			energyUI_->Debug_Gui();
			ImGui::TreePop();
		}
	}

	if (Input::IsTriggerKey(DIK_R)) {
		ResetRail();
		railPointEditer_->SetIsAdd(false);
	}

	ImGui::End();
}

void GameScene::ResetRail() {
	
	railPointEditer_->Init();

	rail_->Finalize();
	rail_->Init("./Game/Resources/", "rail.obj", (uint32_t)railPointEditer_->GetPointsSize());
}

#endif