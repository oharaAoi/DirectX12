#include "GameScene.h"

GameScene::GameScene() {
}

GameScene::~GameScene() {
}

void GameScene::Finalize() {
}

void GameScene::Init() {
	
}

void GameScene::Update() {
	
#ifdef _DEBUG
	Debug_Gui();
#endif
}

void GameScene::Draw() const {
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void GameScene::Debug_Gui() {
	ImGui::Begin("GameScene");
	
	ImGui::End();
}

#endif