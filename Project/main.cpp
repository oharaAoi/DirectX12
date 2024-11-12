#include <Windows.h>
#include "Engine.h"
#include "Engine/ParticleSystem/EffectSystem.h"
#include "Game/Scene/GameScene.h"
#include "Game/Scene/TestScene.h"
#include "Engine/Manager/SceneManager.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	// 出力ウィンドウへの文字出力
	OutputDebugStringA("Hello, DirectX\n");
	D3DResourceLeakChecker debugChecker;

	std::unique_ptr<SceneManager> sceneManager_ = std::make_unique<SceneManager>();
	sceneManager_->Init();
	sceneManager_->Run();

	return 0;
}