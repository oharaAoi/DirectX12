#include "GameScene.h"

GameScene::GameScene() {
}

GameScene::~GameScene() {
}

void GameScene::Init() {
	camera_ = std::make_unique<Camera>();

	sphere_ = Engine::CreateSphere(16);

	model_ = Engine::CreateModel("floor2.obj");
	sphereModel_ = Engine::CreateModel("sphere.obj");
	teapotModel_ = Engine::CreateModel("sphere2.obj");

	transform_ = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
	sphereTransform_ = { {0.5f, 0.5f, 0.5f}, {0.0f, -1.5f, 0.0f}, {0.0f, 0.5f, 0.0f} };
	sphereModelTransform_ = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {1.5f, 0.5f, 0.0f} };
	teapotTransform_ = { {0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {-1.5f, 0.5f, 0.0f} };

	roughness_ = 0.5f;
	metallic_ = 0.5f;
}

void GameScene::Update() {
	//transform_.rotate.y += 0.01f;
	Matrix4x4 triangleWorld = MakeAffineMatrix(transform_);
	Matrix4x4 sphereWorld = MakeAffineMatrix(sphereTransform_);
	Matrix4x4 sphereModelWorld = MakeAffineMatrix(sphereModelTransform_);
	Matrix4x4 teapotWorld = MakeAffineMatrix(teapotTransform_);

	sphere_->Update(sphereWorld, camera_->GetViewMatrix(), camera_->GetProjectionMatrix());

	model_->Update(triangleWorld, camera_->GetViewMatrix(), camera_->GetProjectionMatrix());
	sphereModel_->Update(sphereModelWorld, camera_->GetViewMatrix(), camera_->GetProjectionMatrix());
	teapotModel_->Update(teapotWorld, camera_->GetViewMatrix(), camera_->GetProjectionMatrix());

	camera_->Update();

	Engine::SetEyePos(camera_->GetWorldTranslate());
	//Engine::SetEyePos(camera_->GetTranslate());

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// ImGui
	//////////////////////////////////////////////////////////////////////////////////////////////////
	ImGui::Begin("Setting");
	if (ImGui::CollapsingHeader("transformations")) {
		ImGui::DragFloat3("Translate", &transform_.translate.x, 0.1f);
		ImGui::DragFloat3("rotate", &transform_.rotate.x, 0.1f);
		ImGui::Spacing();

		ImGui::DragFloat3("sTranslate", &sphereTransform_.translate.x, 0.1f);
		ImGui::DragFloat3("sRotate", &sphereTransform_.rotate.x, 0.1f);
		ImGui::DragFloat3("sScale", &sphereTransform_.scale.x, 0.1f);
		ImGui::Spacing();

		ImGui::DragFloat3("tTranslate", &teapotTransform_.translate.x, 0.1f);
		ImGui::DragFloat3("tRotate", &teapotTransform_.rotate.x, 0.1f);
		ImGui::DragFloat3("tScale", &teapotTransform_.scale.x, 0.1f);
	}

	if (ImGui::CollapsingHeader("materials")) {
		sphere_->SetMaterials(roughness_, metallic_);
		model_->SetMaterials(roughness_, metallic_);
		sphereModel_->SetMaterials(roughness_, metallic_);
		teapotModel_->SetMaterials(roughness_, metallic_);

		ImGui::DragFloat("metallic", &metallic_,0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("roughness", &roughness_, 0.01f, 0.0f, 1.0f);
	}
	ImGui::End();

	model_->ImGuiDraw("plane");
	sphere_->ImGuiDraw("Sphere");
	sphereModel_->ImGuiDraw("sphereModel");
	teapotModel_->ImGuiDraw("teapot");
}

void GameScene::Draw() {
	Engine::DrawModel(model_.get());
	
	Engine::DrawSphere(sphere_.get());
	Engine::DrawModel(sphereModel_.get());
	Engine::DrawModel(teapotModel_.get());
}
