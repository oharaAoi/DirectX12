#include "GameScene.h"

GameScene::GameScene() {
}

GameScene::~GameScene() {
}

void GameScene::Init() {
	camera_ = std::make_unique<Camera>();

	triangle_ = Engine::CreateTriangle(vertex_);
	triangle2_ = Engine::CreateTriangle(vertex2_);

	sprite_ = Engine::CreateSprite(rect);

	sphere_ = Engine::CreateSphere(16);

	transform_ = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
}

void GameScene::Update() {
	//transform_.rotate.y += 0.01f;
	Matrix4x4 triangleWorld = MakeAffineMatrix(transform_);

	triangle_->Update(triangleWorld, camera_->GetViewMatrix(), camera_->GetProjectionMatrix());
	triangle2_->Update(triangleWorld, camera_->GetViewMatrix(), camera_->GetProjectionMatrix());

	sprite_->Update(triangleWorld, camera_->GetViewMatrix2D(), camera_->GetProjectionMatrix2D());

	sphere_->Update(triangleWorld, camera_->GetViewMatrix(), camera_->GetProjectionMatrix());

	ImGui::Begin("Setting");

	ImGui::End();
}

void GameScene::Draw() {
	Engine::DrawTriangle(triangle_.get());
	Engine::DrawTriangle(triangle2_.get());

	//Engine::DrawSprite(sprite_.get());

	Engine::DrawSphere(sphere_.get());
}
