#include <Windows.h>
#include "Engine.h"
#include "GameScene.h"
#include "Comparison.h"

static const int kWindowWidth = 1280;
static const int kWindowHeight = 720;

enum Scene {
	kGame,
	kComparison
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	// 出力ウィンドウへの文字出力
	OutputDebugStringA("Hello, DirectX\n");

	D3DResourceLeakChecker debugChecker;

	// 初期化
	Engine::Initialize(kWindowWidth, kWindowHeight);

	std::unique_ptr<GameScene> game = std::make_unique<GameScene>();
	game->Init();

	std::unique_ptr<Comparison> comparison = std::make_unique<Comparison>();

	Scene scene = kComparison;
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

		case kComparison:
			comparison->Update();
			comparison->Draw();
			break;
		}

		ImGui::Begin("Scene");
		ImGui::Combo("sceneKind", &sceneNumber, "Game\0Comparison");
		ImGui::End();


		// ---------------- //
		// 描画処理
		// ---------------- //
		


		Engine::EndFrame();
	}

	Engine::Finalize();

	return 0;
}