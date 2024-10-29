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

	// -------------------------------------------------
	// ↓ GameObjectの初期化
	// -------------------------------------------------
	player_ = std::make_unique<Player>();
	player_->Init();

	// -------------------------------------------------
	// ↓ Editer初期化
	// -------------------------------------------------
	railPointEditer_ = std::make_unique<RailPointEditer>();
	railPointEditer_->Init();

	for (size_t index = 0; index < railPointEditer_->GetRailNum(); ++index) {
		auto& newRail = rails_.emplace_back(std::make_unique<Rail>());
		newRail->Init();
	}

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

	// -------------------------------------------------
	// ↓ 初期化時にやりたい処理を行う
	// -------------------------------------------------
	player_->GetTransform()->SetParent((mainCamera_->GetCameraMatrix()));
	player_->SetPGameScene(this);

	isDebugCamera_ = false;
}

void GameScene::Update() {
	// -------------------------------------------------
	// ↓ Cameraの更新
	// -------------------------------------------------
	mainCamera_->SetControlPoints(railPointEditer_->GetRailPoints());
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

	railPointEditer_->Update();

	// -------------------------------------------------
	// ↓ worldObjectの更新
	// -------------------------------------------------

	skydome_->Update();

	for (size_t index = 0; index < rails_.size(); ++index) {
		rails_[index]->GetTransform()->SetTranslaion(railPointEditer_->GetRailBasePoints()[index]);
		if (index < rails_.size() - 1) {
			Vector3 from = rails_[index]->GetTransform()->GetTranslation();
			Vector3 to = rails_[index + 1]->GetTransform()->GetTranslation();
			Vector3 up = rails_[index]->GetTransform()->GetQuaternion().MakeUp();
			rails_[index]->GetTransform()->SetQuaternion(Quaternion::LookAt(from, to, up));
		}
		rails_[index]->Update();
	}

	// -------------------------------------------------
	// ↓ GameObjectの更新
	// -------------------------------------------------

	// playerの更新
	player_->SetReticlePos(ScreenToWorldCoordinate(reticle_->GetReticle2DPos(), mainCamera_->GetVPVMatrix().Inverse(), 50.0f));
	player_->Update();

	// bulletの更新
	for (auto& bullet : playerBullets_) {
		bullet->Update();
	}

	// -------------------------------------------------
	// ↓ UIの更新
	// -------------------------------------------------
	Vector3 playerPos = Transform({ 0.0, 0.0f, 0.0f }, player_->GetTransform()->GetWorldMatrix());
	reticle_->Update(playerPos, player_->GetForward(), mainCamera_->GetVpvpMatrix());

	// -------------------------------------------------
	// ↓ Managerの更新
	// -------------------------------------------------
	enemyManager_->SetPlayerForward(player_->GetForward());
	enemyManager_->SetPlayerPos(player_->GetWorldPos());
	enemyManager_->Update();

	collisionManager_->Reset();
	for (auto& bullet : playerBullets_) {
		collisionManager_->AddCollider(bullet.get());
	}

	for (auto& enemy : enemyManager_->GetList()) {
		collisionManager_->AddCollider(enemy.get());
	}

	collisionManager_->CheckAllCollision();

	// -------------------------------------------------
	// ↓ 死亡の確認
	// -------------------------------------------------
	playerBullets_.remove_if([](auto& bullet) {return !bullet->GetIsAlive(); });

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
	Engine::SetPipeline(PipelineType::PrimitivePipeline);
	railPointEditer_->Draw(viewMat_ * projectionMat_);

	Engine::SetPipeline(PipelineType::NormalPipeline);
	skydome_->Draw();

	for (size_t index = 0; index < rails_.size(); ++index) {
		rails_[index]->Draw();
	}

	// -------------------------------------------------
	// ↓ GameObjectの描画
	// -------------------------------------------------
	enemyManager_->Draw();

	for (const auto& bullet : playerBullets_) {
		bullet->Draw();
	}

	player_->Draw();

	//mainCamera_->Draw();

	// -------------------------------------------------
	// ↓ UIの描画
	// -------------------------------------------------
	reticle_->Draw();

}

void GameScene::AddPlayerBulletList(const Vector3& pos, const Vector3& velocity) {
	auto& newBullet = playerBullets_.emplace_back(std::make_unique<PlayerBullet>());
	newBullet->Init();
	newBullet->SetPopPos(pos);
	newBullet->SetVelocity(velocity);
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

		if (Input::IsTriggerKey(DIK_1)) {
			isDebugCamera_ = !isDebugCamera_;
		}
	}

	{
		if (ImGui::TreeNode("RailEditer")) {
			railPointEditer_->Debug_Gui();
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

#endif