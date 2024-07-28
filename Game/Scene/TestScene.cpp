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

	skinTransform_.translation_ = { 1.0f, 0.0f, 0.0f };
	sphereTransform_.translation_ = { -1.0f, 0.0f, 0.0f };
	cubeTransform_.translation_ = { 0.0f, 0.0f, 0.0f };

	// モデル --------------------------------------------------------------
	objectKind_ = 0;
	skinModel_ = Engine::CreateModel("skin.obj");
	sphereModel_ = Engine::CreateModel("plane.gltf");
	cubeModel_ = Engine::CreateModel("cube.gltf");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 更新
//////////////////////////////////////////////////////////////////////////////////////////////////
void TestScene::Update() {
	// カメラの更新 -------------------------------------------------------------------
	camera_->Update();
	Engine::SetEyePos(camera_->GetWorldTranslate());
	Engine::SetViewProjection(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());

	// 行列の作成 --------------------------------------------------------------
	skinTransform_.Update();
	sphereTransform_.Update();
	cubeTransform_.Update();

	// gameObjectの更新 ---------------------------------------------------------
	/*skinModel_->Update();
	sphereModel_->Update();
	cubeModel_->Update();*/

	// ImGuiの更新 --------------------------------------------------------
	ImGuiDraw();
}

void TestScene::Draw() {
#pragma region NormalPipeline

	Engine::SetPipeline(PipelineKind::kNormalPipeline);
	/*Engine::DrawModel(skinModel_.get(), skinTransform_);
	Engine::DrawModel(sphereModel_.get(), sphereTransform_);*/
	Engine::DrawModel(cubeModel_.get(), cubeTransform_);

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
