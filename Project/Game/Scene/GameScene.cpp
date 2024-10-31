#include "GameScene.h"

GameScene::GameScene() {
}

GameScene::~GameScene() {
}

void GameScene::Init() {
	// -------------------------------------------------
	// ↓ Cameraの初期化
	// -------------------------------------------------

	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Init();

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

	// -------------------------------------------------
	// ↓ Editer初期化
	// -------------------------------------------------
	
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
	
	// -------------------------------------------------
	// ↓ 初期化時にやりたい処理を行う
	// -------------------------------------------------

	isDebugCamera_ = true;
}

void GameScene::Update() {
	// -------------------------------------------------
	// ↓ Cameraの更新
	// -------------------------------------------------
	
	debugCamera_->Update();

	if (!isDebugCamera_) {
		
	} else {
		Render::SetEyePos(debugCamera_->GetWorldTranslate());
		Render::SetViewProjection(debugCamera_->GetViewMatrix(), debugCamera_->GetProjectionMatrix());
		Render::SetViewProjection2D(debugCamera_->GetViewMatrix2D(), debugCamera_->GetProjectionMatrix2D());
	}

	// -------------------------------------------------
	// ↓ worldObjectの更新
	// -------------------------------------------------

	skydome_->Update();

	field_->Update();

	// -------------------------------------------------
	// ↓ GameObjectの更新
	// -------------------------------------------------


	// -------------------------------------------------
	// ↓ UIの更新
	// -------------------------------------------------
	

	// -------------------------------------------------
	// ↓ Managerの更新
	// -------------------------------------------------
	

	// -------------------------------------------------
	// ↓ 死亡の確認
	// -------------------------------------------------
	
	// -------------------------------------------------
	// ↓ Renderの更新
	// -------------------------------------------------
	if (!isDebugCamera_) {
		
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
	Engine::SetPipeline(PipelineType::NormalPipeline);
	skydome_->Draw();

	field_->Draw();

	// -------------------------------------------------
	// ↓ GameObjectの描画
	// -------------------------------------------------
	
	
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
			ImGui::TreePop();
		}

		if (Input::IsTriggerKey(DIK_1)) {
			isDebugCamera_ = !isDebugCamera_;
		}
	}

	{
		if (ImGui::TreeNode("GameObject")) {
			player_->Debug_Gui();
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
		if (ImGui::TreeNode("UI")) {
		
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

#endif