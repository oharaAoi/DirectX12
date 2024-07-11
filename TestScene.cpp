#include "TestScene.h"

TestScene::TestScene() {
}

TestScene::~TestScene() {
	
}

void TestScene::Init() {
	// カメラ -------------------------------------------------------------------
	camera_ = std::make_unique<Camera>();

	// transform --------------------------------------------------------------
	skinTransform_ = Engine::CreateWorldTransform();

	// モデル --------------------------------------------------------------
	objectKind_ = 0;
	skinModel_ = Engine::CreateModel("skin.obj");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 更新
//////////////////////////////////////////////////////////////////////////////////////////////////
void TestScene::Update() {
	// カメラの更新 -------------------------------------------------------------------
	camera_->Update();
	Engine::SetEyePos(camera_->GetWorldTranslate());

	Engine::SetViewProjection(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());

	// transform --------------------------------------------------------------
	skinTransform_.Update();

	// 行列の作成 --------------------------------------------------------------
	
	// gameObjectの更新 --------------------------------------------------------------
	skinModel_->Update();

	AddGameObject();
}

void TestScene::Draw() {
#pragma region NormalPipeline

	Engine::SetPipeline(PipelineKind::kNormalPipeline);
	Engine::DrawModel(skinModel_.get(), skinTransform_);

#pragma endregion

#pragma region Textureless

	Engine::SetPipeline(PipelineKind::kTexturelessPipeline);

#pragma endregion

#pragma region PBR

	Engine::SetPipeline(PipelineKind::kPBRPipeline);

#pragma endregion

#pragma region Particle

	Engine::SetPipeline(PipelineKind::kParticlePipeline);
	
#pragma endregion

}

void TestScene::AddGameObject() {
	ImGui::Begin("GameObjects");
	ImGui::Combo("object", &objectKind_, "monkey\0sphereModel\0plane\0sphere");
	
	ImGui::End();
}
