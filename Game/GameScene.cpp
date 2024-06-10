#include "GameScene.h"

GameScene::GameScene() {
}

GameScene::~GameScene() {
}

void GameScene::Init() {
	camera_ = std::make_unique<Camera>();

	//sphere_ = Engine::CreateSphere(16);

	model_ = Engine::CreateModel("plane.obj");
	sphereModel_ = Engine::CreateModel("sphere.obj");

	transform_ = { {1.0f, 1.0f, 1.0f}, {1.5f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
	sphereTransform_ = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.5f, 0.0f} };
}

void GameScene::Update() {
	//transform_.rotate.y += 0.01f;
	Matrix4x4 triangleWorld = MakeAffineMatrix(transform_);
	Matrix4x4 sphereWorld = MakeAffineMatrix(sphereTransform_);

	//sphere_->Update(triangleWorld, camera_->GetViewMatrix(), camera_->GetProjectionMatrix());

	model_->Update(triangleWorld, camera_->GetViewMatrix(), camera_->GetProjectionMatrix());
	sphereModel_->Update(sphereWorld, camera_->GetViewMatrix(), camera_->GetProjectionMatrix());

	camera_->Update();

	Engine::SetEyePos(camera_->GetTranslate());

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// ImGui
	//////////////////////////////////////////////////////////////////////////////////////////////////
	ImGui::Begin("Setting");
	ImGui::DragFloat3("Translate", &transform_.translate.x, 0.1f);
	ImGui::DragFloat3("rotate", &transform_.rotate.x, 0.1f);
	ImGui::End();

	model_->ImGuiDraw();
}

void GameScene::Draw() {
	//Engine::DrawSphere(sphere_.get());

	DrawGrid(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());

	Engine::DrawModel(model_.get());
	Engine::DrawModel(sphereModel_.get());
}
