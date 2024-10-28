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

	// -------------------------------------------------
	// ↓ worldObjectの更新
	// -------------------------------------------------

	skydome_->Update();

	// -------------------------------------------------
	// ↓ GameObjectの更新
	// -------------------------------------------------

	// playerの更新
	player_->Update();

	// bulletの更新
	for (auto& bullet : playerBullets_) {
		bullet->Update();
	}

	// -------------------------------------------------
	// ↓ 死亡の確認
	// -------------------------------------------------
	playerBullets_.remove_if([](auto& bullet) {return bullet->GetIsAlive(); });

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
	railPointEditer_->Update();
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

	for (const auto& bullet : playerBullets_) {
		bullet->Draw();
	}

	// -------------------------------------------------
	// ↓ GameObjectの描画
	// -------------------------------------------------
	player_->Draw();

	//mainCamera_->Draw();
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
	ImGui::Checkbox("isDebug", &isDebugCamera_);
	mainCamera_->Debug_Gui();
	debugCamera_->Debug_Gui();
	ImGui::End();
}

#endif