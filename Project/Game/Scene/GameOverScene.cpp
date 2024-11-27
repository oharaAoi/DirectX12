#include "GameOverScene.h"
#include "Engine/Math/Easing.h"

GameOverScene::GameOverScene() {}
GameOverScene::~GameOverScene() {}

void GameOverScene::Finalize() {
	bgm_->Finalize();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameOverScene::Init() {
	adjustmentItem_ = AdjustmentItem::GetInstance();
	adjustmentItem_->Init("GameOverScene");

	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Init();

	camera_ = std::make_unique<Camera>();
	camera_->Init();

	// -------------------------------------------------
	// ↓ worldObjectの初期化
	// -------------------------------------------------
	skydome_ = std::make_unique<Skydome>();
	skydome_->Init();

	backGround_ = std::make_unique<BackGround>();
	backGround_->Init();

	// -------------------------------------------------
	// ↓ GameObjectの初期化
	// -------------------------------------------------
	boss_ = std::make_unique<Boss>();
	boss_->Init();

	bossController_ = std::make_unique<BossController>();
	bossController_->Init(boss_.get());

	// -------------------------------------------------
	// ↓ ライト初期化
	// -------------------------------------------------
	spotLight_ = Render::GetSporLight();
	spotLight_->AddAdjustment();
	spotLight_->AdaptAdjustment();

	firstDistance_ = spotLight_->GetDistance();
	targetDistance_ = 80.0f;

	// -------------------------------------------------
// ↓ audio
// -------------------------------------------------
	bgm_ = std::make_unique<AudioPlayer>();
	bgm_->Init("gameOver.mp3");
	bgm_->Play(false, 0.2f);

	// -------------------------------------------------
	// ↓ パラメータ初期化
	// -------------------------------------------------
	isLight_ = false;

	lightUpTime_ = 0.0f;
	lightUpTimeLimit_ = 3.5f;

	goTitle_ = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameOverScene::Update() {

	if (!isLight_) {
		lightUpTime_ += GameTimer::DeltaTime();
		float t = lightUpTime_ / lightUpTimeLimit_;
		spotLight_->SetDistance(std::lerp(firstDistance_, targetDistance_, Ease::In::Quart(t)));

		if (lightUpTime_ >= lightUpTimeLimit_) {
			isLight_ = true;
			goTitle_ = true;
		}
	}

	boss_->Update();

	if (isLight_) {
		bossController_->Update(goTitle_);
	}
	bossController_->PostUpdate();

	if (Input::IsTriggerKey(DIK_A) || Input::IsTriggerKey(DIK_D)) {
		goTitle_ = !goTitle_;
	}

	if (Input::IsTriggerKey(DIK_SPACE)) {
		if (!goTitle_) {
			nextSceneType_ = SceneType::TITLE;
		} else {
			nextSceneType_ = SceneType::GAME;
		}
	}

	// -------------------------------------------------
	// ↓ worldObjの更新
	// -------------------------------------------------
	skydome_->Update();
	backGround_->Update();

	// -------------------------------------------------
	// ↓ Cameraの更新
	// -------------------------------------------------

	debugCamera_->Update();
	camera_->Update();

	if (!isDebugCamera_) {
		Render::SetEyePos(camera_->GetWorldTranslate());
		Render::SetViewProjection(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());
		Render::SetViewProjection2D(camera_->GetViewMatrix2D(), camera_->GetProjectionMatrix2D());
	} else {
		Render::SetEyePos(debugCamera_->GetWorldTranslate());
		Render::SetViewProjection(debugCamera_->GetViewMatrix(), debugCamera_->GetProjectionMatrix());
		Render::SetViewProjection2D(debugCamera_->GetViewMatrix2D(), debugCamera_->GetProjectionMatrix2D());
	}

	// -------------------------------------------------
	// ↓ Renderの更新
	// -------------------------------------------------
	if (!isDebugCamera_) {
		eyePos_ = camera_->GetWorldTranslate();
		viewMat_ = camera_->GetViewMatrix();
		projectionMat_ = camera_->GetProjectionMatrix();
	} else {
		eyePos_ = debugCamera_->GetWorldTranslate();
		viewMat_ = debugCamera_->GetViewMatrix();
		projectionMat_ = debugCamera_->GetProjectionMatrix();
	}

	Render::SetEyePos(eyePos_);
	Render::SetViewProjection(viewMat_, projectionMat_);
	Render::SetVpvpMat(camera_->GetVPVMatrix());

#ifdef _DEBUG
	Render::Debug_Gui();
	Debug_Gui();
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameOverScene::Draw() const {

	Engine::SetPipeline(PipelineType::NormalPipeline);
	skydome_->Draw();
	backGround_->Draw();

	// -------------------------------------------------
	// ↓ GameObjectの描画
	// -------------------------------------------------
	bossController_->Draw();
	boss_->Draw();

	boss_->PostDraw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug編集
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void GameOverScene::Debug_Gui() {
	ImGui::Begin("GameScene");
	if (ImGui::TreeNode("Camera")) {
		ImGui::Checkbox("isDebug", &isDebugCamera_);

		debugCamera_->Debug_Gui();
		camera_->Debug_Gui();

		if (Input::IsTriggerKey(DIK_C)) {
			isDebugCamera_ = !isDebugCamera_;
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("GameObject")) {
		boss_->Debug_Gui();
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("WorldObject")) {
		backGround_->Debug_Gui();
		bossController_->Debug_Gui();
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("AdjustmentItem")) {
		// Updateだが実質Gui表示なためここで更新
		adjustmentItem_->Update();
		ImGui::TreePop();
	}

	ImGui::End();
}
#endif