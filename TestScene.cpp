#include "TestScene.h"

TestScene::TestScene() {
}

TestScene::~TestScene() {
}

void TestScene::Init() {
	// カメラ -------------------------------------------------------------------
	camera_ = std::make_unique<Camera>();

	// transform --------------------------------------------------------------
	skinTransform_ = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };

	// モデル --------------------------------------------------------------
	skinModel_ = Engine::CreateModel("skin.obj");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 更新
//////////////////////////////////////////////////////////////////////////////////////////////////
void TestScene::Update() {
	// カメラの更新 -------------------------------------------------------------------
	camera_->Update();
	Engine::SetEyePos(camera_->GetWorldTranslate());

	// transform --------------------------------------------------------------
	skinTransform_ = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };

	// 行列の作成 --------------------------------------------------------------
	Matrix4x4 skinWorld = MakeAffineMatrix(skinTransform_);

	// gameObjectの更新 --------------------------------------------------------------
	skinModel_->Update(skinWorld, camera_->GetViewMatrix(), camera_->GetProjectionMatrix());
}

void TestScene::Draw() {
#pragma region NormalPipeline

	Engine::SetPipeline(PipelineKind::kNormalPipeline);

	Engine::DrawModel(skinModel_.get());

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
