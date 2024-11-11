#include "TestScene.h"

TestScene::TestScene() {}
TestScene::~TestScene() {}

void TestScene::Init() {
	// カメラ -------------------------------------------------------------------
	camera_ = std::make_unique<Camera>();
	debugCamera_ = std::make_unique<DebugCamera>();

	// GameObject -------------------------------------------------------------------
	test_animationCS_ = std::make_unique<BaseGameObject>();
	test_animationCS_->Init();
	test_animationCS_->SetObject("amimationCharacter.gltf");
	test_animationCS_->SetAnimater("./Engine/Resources/Animation/", "amimationCharacter.gltf", true);

	//test_animationCS_->SetObjectAxis();

	meshCollider_.Init(test_animationCS_->GetModel()->GetMesh(0));
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 更新
//////////////////////////////////////////////////////////////////////////////////////////////////
void TestScene::Update() {
	// -------------------------------------------------
	// ↓ カメラの更新
	// -------------------------------------------------
	if (isDebugCamera_) {
		debugCamera_->Update();
		Render::SetEyePos(debugCamera_->GetWorldTranslate());
		Render::SetViewProjection(debugCamera_->GetViewMatrix(), debugCamera_->GetProjectionMatrix());
	} else {
		camera_->Update();
		Render::SetEyePos(camera_->GetWorldTranslate());
		Render::SetViewProjection(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());
		Render::SetViewProjection2D(camera_->GetViewMatrix2D(), camera_->GetProjectionMatrix2D());
	}

	// -------------------------------------------------
	// ↓ GameObjectの更新
	// -------------------------------------------------
	test_animationCS_->Update();

	// -------------------------------------------------
	// ↓ ParticleのViewを設定する
	// -------------------------------------------------
	EffectSystem::GetInstacne()->SetCameraMatrix(camera_->GetCameraMatrix());
	EffectSystem::GetInstacne()->SetViewProjectionMatrix(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());

#ifdef _DEBUG
	ImGuiDraw();
#endif
}

void TestScene::Draw() const {
	Engine::SetPipeline(PipelineType::NormalPipeline);
	test_animationCS_->Draw();
}

#ifdef _DEBUG
void TestScene::ImGuiDraw() {
	ImGui::Begin("TestScene");
	ImGui::Checkbox("isDebug", &isDebugCamera_);
	camera_->Debug_Gui();
	debugCamera_->Debug_Gui();

	ImGui::Separator();

	ShowEasingDebug(easeIndex_);

	ImGui::End();
}
#endif
