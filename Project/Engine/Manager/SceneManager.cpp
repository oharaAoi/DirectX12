#include "SceneManager.h"

SceneManager::SceneManager() {}

SceneManager::~SceneManager() {}

void SceneManager::Finalize() {
	effectSystem_->Finalize();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////
void SceneManager::Init() {
	// gameに必要なResourceの読み込み
	resources_.Load();

	Render::Begin();

	scene_ = std::make_unique<GameScene>();
	scene_->Init();

	effectSystem_ = EffectSystem::GetInstacne();
	effectSystem_->Init();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void SceneManager::Update() {
	scene_->Update();
	effectSystem_->Update();
	Render::Update();

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void SceneManager::Draw() {
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

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　シーンの切り替え
//////////////////////////////////////////////////////////////////////////////////////////////////

void SceneManager::SetChange() {
	GetScene();
	scene_->Init();
}

void SceneManager::GetScene() {
	
}
