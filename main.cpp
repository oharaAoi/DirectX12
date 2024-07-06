#include <Windows.h>
#include "Engine.h"
#include "GameScene.h"
#include "Comparison.h"
#include "Metallic.h"

static const int kWindowWidth = 1280;
static const int kWindowHeight = 720;

enum Scene {
	kGame,
	kComparison,
	kMetallic
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	// 出力ウィンドウへの文字出力
	OutputDebugStringA("Hello, DirectX\n");

	D3DResourceLeakChecker debugChecker;

	// 初期化
	Engine::Initialize(kWindowWidth, kWindowHeight);

	std::unique_ptr<GameScene> game = std::make_unique<GameScene>();
	game->Init();

	int sceneNumber = 0;

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
			
			break;
		case kMetallic:
			
			break;
		}

		ImGui::Begin("Scene");
		ImGui::Combo("sceneKind", &sceneNumber, "Game\0Comparison\0Metallic");
		ImGui::End();


		// ---------------- //
		// 描画処理
		// ---------------- //
		


		Engine::EndFrame();
	}

	Engine::Finalize();

	return 0;
}