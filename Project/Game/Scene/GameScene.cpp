#include "GameScene.h"

GameScene::GameScene() {
}

GameScene::~GameScene() {
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
	bossAttackEditer_ = std::make_unique<BossAttackEditer>();
	bossAttackEditer_->Init();

	// -------------------------------------------------
	// ↓ UI初期化
	// -------------------------------------------------
	
	// -------------------------------------------------
	// ↓ 初期化時にやりたい処理を行う
	// -------------------------------------------------

	isDebugCamera_ = true;
	followCamera_->SetTarget(player_->GetTransform());

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
	boss_->Update();

	if (!isDebugCamera_) {
		player_->SetInverMatrix(followCamera_->GetVPVMatrix().Inverse());
		player_->SetCameraZDis(followCamera_->GetTranslate().z);
		player_->Update();
	}

	testCollisionObj_->Update();

	// -------------------------------------------------
	// ↓ UIの更新
	// -------------------------------------------------
	

	// -------------------------------------------------
	// ↓ Managerの更新
	// -------------------------------------------------
	
	collisionManager_->Reset();
	collisionManager_->AddCollider(player_->GetWireTipCollider());
	collisionManager_->AddCollider(testCollisionObj_.get());
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

#ifdef _DEBUG
	Debug_Gui();
#endif
}

void GameScene::Draw() const {
	// -------------------------------------------------
	// ↓ Debugの描画
	// -------------------------------------------------
#ifdef _DEBUG
	bossAttackEditer_->Draw();
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
	// -------------------------------------------------
	// ↓ UIの描画
	// -------------------------------------------------
	
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

		if (Input::IsTriggerKey(DIK_1)) {
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

	bossAttackEditer_->Update();
	{
		if (ImGui::TreeNode("BossAttackEditer")) {
			ImGui::Begin("BossAttackEditer");
			bossAttackEditer_->AddPoint();
			ImGui::End();
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

#endif