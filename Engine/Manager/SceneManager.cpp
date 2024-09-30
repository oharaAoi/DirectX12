#include "SceneManager.h"

SceneManager::SceneManager() { Init();}

SceneManager::~SceneManager() {}

void SceneManager::Finalize() {
	effectSystem_->Finalize();
	ModelManager::GetInstance()->Finalize();
	Engine::Finalize();
}

void SceneManager::Init() {
	Engine::Initialize(kWindowWidth_, kWindowHeight_);
	ModelManager::GetInstance()->Init();

	scene_ = std::make_unique<TestScene>();
	scene_->Load();
	scene_->Init();

	effectSystem_ = EffectSystem::GetInstacne();
	effectSystem_->Init();
}

void SceneManager::Run() {
	while (Engine::ProcessMessage()) {
		Engine::BeginFrame();

		// ------------------------------------ //
		// 更新処理
		// ------------------------------------ //
		scene_->Update();

		effectSystem_->Update();

		Render::Update();

		// ------------------------------------ //
		// 描画処理
		// ------------------------------------ //

		scene_->Draw();

		Engine::SetPipeline(PipelineKind::kParticlePipeline);
		effectSystem_->Draw();

		#ifdef _DEBUG
		effectSystem_->BeginEditer();
		effectSystem_->UpdateEditer();
		Engine::SetPipeline(PipelineKind::kParticlePipeline);
		effectSystem_->DrawEditer();
		#endif

		// ------------------------------------ //
		// フレーム終了処理
		// ------------------------------------ //
		Engine::EndRenderTexture();
		Engine::EndImGui();

		#ifdef _DEBUG
		effectSystem_->EndEditer();
		#endif

		Engine::EndFrame();

		//gameTimer_.WaitNextFrame();
	}

	Finalize();
}
