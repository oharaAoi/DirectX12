#include "AoiFramework.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　終了処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void AoiFramework::Finalize() {
	MeshManager::GetInstance()->Finalize();
	ModelManager::GetInstance()->Finalize();
	Engine::Finalize();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void AoiFramework::Init() {
	Engine::Initialize(kWindowWidth_, kWindowHeight_);
	ModelManager::GetInstance()->Init();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void AoiFramework::Update() {
	Engine::BeginFrame();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　ゲーム処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void AoiFramework::Run() {
	Init();

	while (Engine::ProcessMessage()) {

		// 更新
		Update();

		if (isEndRequest()) {
			break;
		}

		// 描画
		Draw();
	}

	Finalize();
}
