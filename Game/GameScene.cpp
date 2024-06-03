#include "GameScene.h"

GameScene::GameScene() {
}

GameScene::~GameScene() {
}

void GameScene::Init() {
	camera_ = std::make_unique<Camera>();

	sphere_ = Engine::CreateSphere(16);

	model_ = Engine::CreateModel("suzanne.obj");

	transform_ = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
}

void GameScene::Update() {
	//transform_.rotate.y += 0.01f;
	Matrix4x4 triangleWorld = MakeAffineMatrix(transform_);

	sphere_->Update(triangleWorld, camera_->GetViewMatrix(), camera_->GetProjectionMatrix());

	model_->Update(triangleWorld, camera_->GetViewMatrix(), camera_->GetProjectionMatrix());

	camera_->Update();

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// ImGui
	//////////////////////////////////////////////////////////////////////////////////////////////////
	ImGui::Begin("Setting");
	
	ImGui::End();
}

void GameScene::Draw() {
	//Engine::DrawSphere(sphere_.get());

	DrawGrid(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());

	Engine::DrawModel(model_.get());
}
