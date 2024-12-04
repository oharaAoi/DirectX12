#include "GameScene.h"
#include "Engine/Utilities/AdjustmentItem.h"

GameScene::GameScene() {
}

GameScene::~GameScene() {
}

void GameScene::Finalize() {
}

void GameScene::Init() {
	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	adjust->Init("GameScene");

	debugCamera_ = std::make_unique<DebugCamera>();

	ground_ = std::make_unique<Ground>();
	ground_->Init();
}

void GameScene::Update() {
	// -------------------------------------------------
	// ↓ カメラの更新
	// -------------------------------------------------

	debugCamera_->Update();
	Render::SetEyePos(debugCamera_->GetWorldTranslate());
	Render::SetViewProjection(debugCamera_->GetViewMatrix(), debugCamera_->GetProjectionMatrix());

	// -------------------------------------------------
	// ↓ GameObjectの更新
	// -------------------------------------------------

	ground_->Update();

}

void GameScene::Draw() const {
	Engine::SetPipeline(PipelineType::NormalPipeline);
	ground_->Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void GameScene::Debug_Gui() {
	ImGui::Begin("GameScene");
	if (ImGui::TreeNode("World")) {
		ground_->Debug_Gui();
		ImGui::TreePop();
	}
	ImGui::End();
}

#endif