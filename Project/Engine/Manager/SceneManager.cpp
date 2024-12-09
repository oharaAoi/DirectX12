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

	effectSystem_->Draw(Engine::GetIsOpenEffectEditer());

	Engine::RenderFrame();
}

void SceneManager::PostFrame() {
	// ------------------------------------ //
	// フレーム終了処理
	// ------------------------------------ //
	Engine::EndImGui();
#ifdef _DEBUG
	effectSystem_->PostDraw(Engine::GetIsOpenEffectEditer());
#endif
	Engine::EndFrame();

	// fpsの計算
	gameTimer_.CalculationFrame();
}

#ifdef _DEBUG
void SceneManager::Debug_Gui() {
	if (ImGui::Begin("My Window", nullptr, ImGuiWindowFlags_MenuBar)) {
		scene_->Debug_Gui();
	}

	ImGui::Begin("GameTimer");
	gameTimer_.FPS();
	ImGui::End();

	ImGui::End();
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