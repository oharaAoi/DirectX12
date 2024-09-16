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
	sphereTransform_ = Engine::CreateWorldTransform();
	cubeTransform_ = Engine::CreateWorldTransform();

	skinTransform_.translation_ = { 2.0f, 0.0f, 0.0f };
	sphereTransform_.translation_ = { -2.0f, 0.0f, 0.0f };
	cubeTransform_.translation_ = { 0.0f, 0.0f, 0.0f };
	cubeTransform_.scale_ = { 0.6f, 0.6f, 0.6f };

	// モデル --------------------------------------------------------------
	objectKind_ = 0;
	skinModel_ = Engine::CreateModel("skin.obj");
	sphereModel_ = Engine::CreateModel("sphere_Block.obj");
	//cubeModel_ = Engine::CreateModel("cube.obj");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 更新
//////////////////////////////////////////////////////////////////////////////////////////////////
void TestScene::Update() {
	// -------------------------------------------------
	// ↓ カメラの更新
	// -------------------------------------------------
	camera_->Update();
	Render::SetEyePos(camera_->GetWorldTranslate());
	Render::SetViewProjection(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());
	Render::SetViewProjection2D(camera_->GetViewMatrix2D(), camera_->GetProjectionMatrix2D());

	// -------------------------------------------------
	// ↓ ParticleのViewを設定する
	// -------------------------------------------------
	EffectSystem::GetInstacne()->SetCameraMatrix(camera_->GetCameraMatrix());
	EffectSystem::GetInstacne()->SetViewProjectionMatrix(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());

	// -------------------------------------------------
	// ↓ 行列の更新
	// -------------------------------------------------
	skinTransform_.Update();
	sphereTransform_.Update();
	cubeTransform_.Update();

	// -------------------------------------------------
	// ↓ の更新
	// -------------------------------------------------
	ImGuiDraw();
}

void TestScene::Draw() {
#pragma region NormalPipeline

	Engine::SetPipeline(PipelineKind::kNormalPipeline);
	Render::DrawModel(skinModel_.get(), skinTransform_);
	//Engine::DrawModel(cubeModel_.get(), cubeTransform_);

#pragma endregion

#pragma region Textureless

	Engine::SetPipeline(PipelineKind::kTexturelessPipeline);

#pragma endregion

#pragma region PBR

	Engine::SetPipeline(PipelineKind::kPBRPipeline);
	Render::DrawModel(sphereModel_.get(), sphereTransform_);

#pragma endregion

#pragma region Particle

	Engine::SetPipeline(PipelineKind::kParticlePipeline);
	
#pragma endregion

}

void TestScene::ImGuiDraw() {
	ImGui::Begin("GameObjects");
	if (ImGui::TreeNode("skin")) {
		skinTransform_.ImGuiDraw();
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("sphere")) {
		sphereTransform_.ImGuiDraw();
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("cube")) {
		cubeTransform_.ImGuiDraw();
		ImGui::TreePop();
	}
	
	ImGui::End();
}
