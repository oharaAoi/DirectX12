#include <Windows.h>
#include "Engine.h"
#include "Engine/ParticleSystem/EffectSystem.h"
#include "Game/Scene/GameScene.h"
#include "Game/Scene/TestScene.h"
#include "Game/Scene/TaskScene.h"

enum Scene {
	kGame,
	kTest,
	kTask
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	// 出力ウィンドウへの文字出力
	OutputDebugStringA("Hello, DirectX\n");

	D3DResourceLeakChecker debugChecker;

	// 初期化
	Engine::Initialize(kWindowWidth_, kWindowHeight_);

	// effectSystem
	EffectSystem* effectSystem = EffectSystem::GetInstacne();
	effectSystem->Init();

	std::unique_ptr<GameScene> game = std::make_unique<GameScene>();
	std::unique_ptr<TestScene> test = std::make_unique<TestScene>();
	std::unique_ptr<TaskScene> task = std::make_unique<TaskScene>();

	game->Init();
	test->Init();
	task->Init();

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

		case kTask:
			task->Update();
			task->Draw();
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

		Engine::SetPipeline(PipelineKind::kParticlePipeline);
		effectSystem->Draw();

		Render::Update();
		
		// offScreenのスプライトを描画する
		Engine::EndRenderTexture();

		Engine::EndFrame();
	}

	effectSystem->Finalize();
	Engine::Finalize();

	return 0;
}