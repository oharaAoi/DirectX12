#include <Windows.h>
#include "Engine.h"
#include "GameScene.h"

static const int kWindowWidth = 1280;
static const int kWindowHeight = 720;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	// 出力ウィンドウへの文字出力
	OutputDebugStringA("Hello, DirectX\n");

	// 初期化
	Engine::Initialize(kWindowWidth, kWindowHeight);

	std::unique_ptr<GameScene> game = std::make_unique<GameScene>();
	game->Init();

	// mainループ
	while (Engine::ProcessMessage()) {
		Engine::BeginFrame();

		// ---------------- //
		// 更新処理
		// ---------------- //
		game->Update();

		// ---------------- //
		// 描画処理
		// ---------------- //
		game->Draw();


		Engine::EndFrame();
	}

	return 0;
}