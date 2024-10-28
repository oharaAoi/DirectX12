#include "GameScene.h"

GameScene::GameScene() {
}

GameScene::~GameScene() {
}

void GameScene::Init() {
	camera_ = std::make_unique<Camera>();

	sphere_ = Engine::CreateSphere(16);

	roughness_ = 0.5f;
	metallic_ = 0.5f;

	lightKind_ = 0;
}

void GameScene::Load() {
}

void GameScene::Update() {
	//transform_.rotate.y += 0.01f;

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// カメラの更新
	//////////////////////////////////////////////////////////////////////////////////////////////////
	camera_->Update();
	Render::SetEyePos(camera_->GetWorldTranslate());
	Render::SetViewProjection(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());


}

void GameScene::Draw() const {

}
