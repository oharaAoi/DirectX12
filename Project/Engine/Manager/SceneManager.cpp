#include "SceneManager.h"

SceneManager::SceneManager() {}

SceneManager::~SceneManager() {}

void SceneManager::Finalize() {
	effectSystem_->Finalize();
	MeshManager::GetInstance()->Finalize();
	ModelManager::GetInstance()->Finalize();
	Engine::Finalize();
}

void SceneManager::Init() {
	Engine::Initialize(kWindowWidth_, kWindowHeight_);
	ModelManager::GetInstance()->Init();

	// gameに必要なResourceの読み込み
	resources_.Load();

	scene_ = std::make_unique<TestScene>();
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

		Engine::SetPipeline(PipelineType::ParticlePipeline);
		effectSystem_->Draw();

		#ifdef _DEBUG
		// ------------------------------------ //
		// EffectEditerの処理
		// ------------------------------------ //

		effectSystem_->Debug_Gui();
		if (effectSystem_->GetIsEffectEditer()) {
			effectSystem_->BeginEditer();
			effectSystem_->UpdateEditer();
			Engine::SetPipeline(PipelineType::ParticlePipeline);
			effectSystem_->DrawEditer();
		}

		#endif
		gameTimer_.FPS();

		// ------------------------------------ //
		// フレーム終了処理
		// ------------------------------------ //
		Engine::DrawRenderTexture();
		Engine::EndImGui();

		#ifdef _DEBUG
		if (effectSystem_->GetIsEffectEditer()) {
			effectSystem_->EndEditer();
		}
		#endif

		Engine::EndFrame();

		// fpsの計算
		gameTimer_.CalculationFrame();
	}

	Finalize();
}

void SceneManager::SetChange() {
	GetScene();
	scene_->Init();
}

void SceneManager::GetScene() {
	
}
