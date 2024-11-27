#include "GameClearScene.h"

GameClearScene::GameClearScene() {}
GameClearScene::~GameClearScene() {}

void GameClearScene::Finalize() {
	bgm_->Finalize();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameClearScene::Init() {
	adjustmentItem_ = AdjustmentItem::GetInstance();
	adjustmentItem_->Init("clearScene");

	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Init();

	skydome_ = std::make_unique<Skydome>();
	skydome_->Init();


	player_ = std::make_unique<TitlePlayer>();
	player_->Init();
	player_->ClearSet();


	panel_ = std::make_unique<Panel>();
	panel_->Init();
	panel_->SetBlackOutOpen();

	clearUI_ = Engine::CreateSprite("clearUI.png");
	clearUI_->SetScale(Vector2{ 0.5f, 0.5f });
	clearUI_->SetTranslate({ 400.0f,280.0f });

	goTitle_= Engine::CreateSprite("goTitle.png");
	goTitle_->SetScale(Vector2{ 0.3f, 0.3f });
	goTitle_->SetTranslate(Vector2{ 360.0f, 600.0f });

	// -------------------------------------------------
	// ↓ audio
	// -------------------------------------------------
	bgm_ = std::make_unique<AudioPlayer>();
	bgm_->Init("gameClear.mp3");
	bgm_->Play(true, 0.2f);

	SpotLight* spotLight = Render::GetSporLight();
	spotLight->AddAdjustment();
	spotLight->AdaptAdjustment();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameClearScene::Update() {
	skydome_->Update();

	clearUI_->Update();
	goTitle_->Update();


	if (isNextScene_) {
		AutoUpdate();
	}
	else {
		ClearUpdate();
	}

	panel_->Update();
	followCamera_->Update();

	Render::SetEyePos(followCamera_->GetWorldTranslate());
	Render::SetViewProjection(followCamera_->GetViewMatrix(), followCamera_->GetProjectionMatrix());
	Render::SetViewProjection2D(followCamera_->GetViewMatrix2D(), followCamera_->GetProjectionMatrix2D());




	// -------------------------------------------------
	// ↓ Renderの更新
	// -------------------------------------------------
	eyePos_ = followCamera_->GetWorldTranslate();
	viewMat_ = followCamera_->GetViewMatrix();
	projectionMat_ = followCamera_->GetProjectionMatrix();

	Render::SetEyePos(eyePos_);
	Render::SetViewProjection(viewMat_, projectionMat_);
	Render::SetVpvpMat(followCamera_->GetVPVMatrix());

#ifdef _DEBUG
	Render::Debug_Gui();
	ImGuiDraw();
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameClearScene::Draw() const {
	Engine::SetPipeline(PipelineType::NormalPipeline);

	skydome_->Draw();

	player_->Draw();


	Engine::SetPipeline(PipelineType::SpriteNormalBlendPipeline);
	clearUI_->Draw();
	goTitle_->Draw();
	panel_->Draw();
}

void GameClearScene::DrawUI() {
	Engine::SetPipeline(PipelineType::SpriteNormalBlendPipeline);

}



void GameClearScene::AutoUpdate() {
	if (panel_->GetIsFinished()) {
		nextSceneType_ = SceneType::TITLE;
		return;
	}

	if (isNextScene_) {
		// ここでブラックアウトさせる
		if (panel_->GetDoNoting()) {
			panel_->SetBlackOut();
		}
	}
}

void GameClearScene::ClearUpdate() {
	player_->SetInverMatrix(followCamera_->GetVPVMatrix().Inverse());
	player_->SetCameraZDis(followCamera_->GetTranslate().z);

	if (Input::IsTriggerKey(DIK_SPACE) || Input::IsTriggerMouse(0)) {
		isNextScene_ = true;
		Input::SetNotAccepted(true);
	}

	player_->ClearUpdate();

	//Vector3 playerPos = player_->GetTransform()->GetTranslation();
	//if (playerPos.y >= 18.0f) {
	//	// ここでブラックアウトさせる
	//	isNextScene_ = true;
	//	Input::SetNotAccepted(true);
	//}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug編集
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void GameClearScene::ImGuiDraw() {
	ImGui::Begin("ClearScene");

	if (ImGui::TreeNode("AdjustmentItem")) {
		// Updateだが実質Gui表示なためここで更新
		adjustmentItem_->Update();
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("sprite")) {
		clearUI_->Debug_Gui("clearUI");
		goTitle_->Debug_Gui("goTitle");
		ImGui::TreePop();
	}

	ImGui::End();
}
#endif