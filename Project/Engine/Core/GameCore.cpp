#include "GameCore.h"

GameCore::GameCore() {}
GameCore::~GameCore() {}


//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　終了処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameCore::Finalize() {
	sceneManger_->Finalize();
	AoiFramework::Finalize();
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////
void GameCore::Init() {
	AoiFramework::Init();

	sceneManger_ = std::make_unique<SceneManager>();
	sceneManger_->Init();

	sceneManger_->SetChange(SceneType::GAME);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameCore::Update() {
	AoiFramework::Update();
	sceneManger_->Update();

#ifdef _DEBUG
	Debug_Gui();
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameCore::Draw() {
	sceneManger_->Draw();
	sceneManger_->PostFrame();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug処理
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void GameCore::Debug_Gui() {
	sceneManger_->Debug_Gui();
}
#endif