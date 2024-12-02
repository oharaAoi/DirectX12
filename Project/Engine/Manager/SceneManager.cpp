#include "SceneManager.h"
#include <optional>

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

	sceneFactory_ = std::make_unique<SceneFactory>();

	effectSystem_ = EffectSystem::GetInstacne();
	effectSystem_->Init();

	Render::Begin();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void SceneManager::Update() {
	if (scene_->GetNextSceneType()) {
		SetChange(scene_->GetNextSceneType().value());
		scene_->SetNextSceneType(std::nullopt);
	}

	scene_->Update();
	effectSystem_->Update();
	Render::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void SceneManager::Draw() {
	scene_->Draw();
	effectSystem_->Draw();
	Engine::DrawRenderTexture();
}

void SceneManager::PostFrame() {
	gameTimer_.FPS();

	// ------------------------------------ //
	// フレーム終了処理
	// ------------------------------------ //
	Engine::EndImGui();

	effectSystem_->PostDraw();

	Engine::EndFrame();

	// fpsの計算
	gameTimer_.CalculationFrame();
}

#ifdef _DEBUG
void SceneManager::Debug_Gui() {
	scene_->Debug_Gui();
	effectSystem_->Debug_Gui();
}
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　シーンの切り替え
//////////////////////////////////////////////////////////////////////////////////////////////////

void SceneManager::SetChange(const SceneType& type) {
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);
	nextScene_ = sceneFactory_->CreateScene(sceneFactory_->SceneTypeToString(type));
	nextScene_->Init();

	if (scene_ != nullptr) {
		scene_->Finalize();
	}
	scene_ = std::move(nextScene_);
}