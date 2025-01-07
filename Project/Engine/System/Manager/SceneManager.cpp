#include "SceneManager.h"
#include "Engine/Editer/Window/EditerWindows.h"
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

	Engine::RenderFrame();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　フレーム終了処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void SceneManager::PostFrame() {
	Engine::EndImGui();
	Engine::EndFrame();

	// fpsの計算
	gameTimer_.CalculationFrame();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　編集処理
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void SceneManager::Debug_Gui() {
	ImGui::Begin("GameTimer", nullptr);
	gameTimer_.FPS();
	ImGui::End();

	// sceneのDebug
	ImGui::Begin("Scene");
	// 切り替える
	if (ImGui::Button("change")) {
		scene_->SetNextSceneType(changeScene_);
	}
	ImGui::SameLine();
	// 選択する
	int currentIndex = static_cast<int>(changeScene_);
	// ImGui::Comboを使用して選択可能にする
	if (ImGui::Combo("Scene Type", &currentIndex, kSceneTypeNames, IM_ARRAYSIZE(kSceneTypeNames))) {
		// ユーザーが選択を変更した場合にSceneTypeを更新
		changeScene_ = static_cast<SceneType>(currentIndex);
	}
	scene_->Debug_Gui();
	ImGui::End();
}
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　シーンの切り替え
//////////////////////////////////////////////////////////////////////////////////////////////////

void SceneManager::SetChange(const SceneType& type) {
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);
	EditerWindows::GetInstance()->Clear();
	nextScene_ = sceneFactory_->CreateScene(sceneFactory_->SceneTypeToString(type));
	nextScene_->Init();

	if (scene_ != nullptr) {
		scene_->Finalize();
	}
	scene_ = std::move(nextScene_);
}