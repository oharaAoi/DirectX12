#include "TitleScene.h"
#include "Engine/Input/Input.h"

TitleScene::TitleScene() {}
TitleScene::~TitleScene() {}

void TitleScene::Finalize() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void TitleScene::Init() {

	adjustmentItem_ = AdjustmentItem::GetInstance();
	adjustmentItem_->Init("titleScene");

	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Init();

	skydome_ = std::make_unique<Skydome>();
	skydome_->Init();


	player_ = std::make_unique<TitlePlayer>();
	player_->Init();
	player_->TitleSet();


	panel_ = std::make_unique<Panel>();
	panel_->Init();


	SpotLight* spotLight = Render::GetSporLight();
	spotLight->AddAdjustment();
	spotLight->AdaptAdjustment();

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void TitleScene::Update() {

	skydome_->Update();

	if (isNextScene_) {
		AutoUpdate();
	}
	else {
		TitleUpdate();
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

void TitleScene::Draw() const {
	Engine::SetPipeline(PipelineType::NormalPipeline);

	skydome_->Draw();

	player_->Draw();


	Engine::SetPipeline(PipelineType::SpriteNormalBlendPipeline);
	panel_->Draw();

}

void TitleScene::AutoUpdate() {
	if (panel_->GetIsFinished()) {
		nextSceneType_ = SceneType::TUTORIAL;
		return;
	}

	Vector3 playerPos = player_->GetTransform()->GetTranslation();
	if (playerPos.y >= 18.0f) {
		// ここでブラックアウトさせる
		if (panel_->GetDoNoting()) {
			panel_->SetBlackOut();
		}
	}
}

void TitleScene::TitleUpdate() {
	player_->SetInverMatrix(followCamera_->GetVPVMatrix().Inverse());
	player_->SetCameraZDis(followCamera_->GetTranslate().z);

	if (Input::IsTriggerKey(DIK_SPACE)) {
		player_->TitleEnd();
	}

	player_->Update();

	Vector3 playerPos = player_->GetTransform()->GetTranslation();
	if (playerPos.y >= 18.0f) {
		// ここでブラックアウトさせる
		isNextScene_ = true;
		Input::SetNotAccepted(true);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug編集
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void TitleScene::ImGuiDraw() {
	ImGui::Begin("TitleScene");

	if (ImGui::TreeNode("AdjustmentItem")) {
		// Updateだが実質Gui表示なためここで更新
		adjustmentItem_->Update();
		ImGui::TreePop();
	}

	ImGui::End();
}
#endif
