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

	player_ = std::make_unique<Player>();
	player_->Init();
	player_->TitleSet();

	SpotLight* spotLight = Render::GetSporLight();
	spotLight->AddAdjustment();
	spotLight->AdaptAdjustment();

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void TitleScene::Update() {
	if (Input::IsTriggerKey(DIK_SPACE)) {
		player_->TitleEnd();
	}
	if (player_->GetTransform()->GetTranslation().y >= 18.0f) {
		nextSceneType_ = SceneType::TUTORIAL;
	}
	player_->TitleUpdate();

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

	player_->Draw();
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