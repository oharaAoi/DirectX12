#include "GameScene.h"

GameScene::GameScene() {
}

GameScene::~GameScene() {
}

void GameScene::Init() {
	camera_ = std::make_unique<Camera>();

	triangle_ = Engine::CreateTriangle(vertex_);

	transform_ = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
}

void GameScene::Update() {
	transform_.rotate.y += 0.01f;
	Matrix4x4 triangleWorld = MakeAffineMatrix(transform_);

	triangle_->Update(triangleWorld, camera_->GetViewMatrix(), camera_->GetProjectionMatrix());
}

void GameScene::Draw() {
	Engine::DrawTriangle(triangle_.get());
}
