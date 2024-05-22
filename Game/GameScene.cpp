#include "GameScene.h"

GameScene::GameScene() {
}

GameScene::~GameScene() {
}

void GameScene::Init() {
	triangle_ = Engine::CreateTriangle(vertex_);
}

void GameScene::Update() {
}

void GameScene::Draw() {
	Engine::DrawTriangle(triangle_.get());
}
