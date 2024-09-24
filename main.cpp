#include <Windows.h>
#include "Engine.h"
#include "Engine/ParticleSystem/EffectSystem.h"
#include "Game/Scene/GameScene.h"
#include "Game/Scene/TestScene.h"

enum Scene {
	kGame,
	kTest,
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	// 出力ウィンドウへの文字出力
	OutputDebugStringA("Hello, DirectX\n");

	D3DResourceLeakChecker debugChecker;

	// 初期化
	EffectSystem* effectSystem = EffectSystem::GetInstacne();
	Engine::Initialize(kWindowWidth_, kWindowHeight_);

	// effectSystem
	effectSystem->Init();

	std::unique_ptr<GameScene> game = std::make_unique<GameScene>();
	std::unique_ptr<TestScene> test = std::make_unique<TestScene>();
	
	game->Init();
	test->Init();
	
	int sceneNumber = 1;

	// mainループ
	while (Engine::ProcessMessage()) {
		Engine::BeginFrame();

		// ------------------------------------ //
		// 更新処理
		// ------------------------------------ //

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
		ImGui::Combo("sceneKind", &sceneNumber, "Game\0Test\0Task");
		ImGui::End();

		// ------------------------------------ //
		// EffectSystemの更新
		// ------------------------------------ //
		effectSystem->Update();

		// ------------------------------------ //
		// 描画処理
		// ------------------------------------ //

		Render::Update();

		Engine::SetPipeline(PipelineKind::kParticlePipeline);
		effectSystem->Draw();

		effectSystem->BeginEditer();

		// offScreenのスプライトを描画する
		Engine::EndRenderTexture();

		Engine::EndImGui();
		effectSystem->EndEditer();

		Engine::EndFrame();
	}

	effectSystem->Finalize();
	Engine::Finalize();

	return 0;
}