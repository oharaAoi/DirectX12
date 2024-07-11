#include <Windows.h>
#include "Engine.h"
#include "GameScene.h"
#include "TestScene.h"

static const int kWindowWidth = 1280;
static const int kWindowHeight = 720;

enum Scene {
	kGame,
	kTest
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	// 出力ウィンドウへの文字出力
	OutputDebugStringA("Hello, DirectX\n");

	D3DResourceLeakChecker debugChecker;

	// 初期化
	Engine::Initialize(kWindowWidth, kWindowHeight);

	std::unique_ptr<GameScene> game = std::make_unique<GameScene>();
	std::unique_ptr<TestScene> test = std::make_unique<TestScene>();

	game->Init();
	test->Init();

	int sceneNumber = 1;

	// mainループ
	while (Engine::ProcessMessage()) {
		Engine::BeginFrame();

		// ---------------- //
		// 更新処理
		// ---------------- //

		switch (sceneNumber) {
		case kGame:
			game->Update();
			game->Draw();
			break;

		case kTest:
			test->Update();
			test->Draw();
			break;
		}

		ImGui::Begin("Scene");
		ImGui::Combo("sceneKind", &sceneNumber, "Game\0Test");
		ImGui::End();


		// ---------------- //
		// 描画処理
		// ---------------- //
		


		Engine::EndFrame();
	}

	Engine::Finalize();

	return 0;
}