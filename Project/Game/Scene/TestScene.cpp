#include "TestScene.h"

TestScene::TestScene() {}
TestScene::~TestScene() {}

void TestScene::Init() {
	// カメラ -------------------------------------------------------------------
	camera_ = std::make_unique<Camera>();
	debugCamera_ = std::make_unique<DebugCamera>();
	
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
	// ↓ ParticleのViewを設定する
	// -------------------------------------------------
	EffectSystem::GetInstacne()->SetCameraMatrix(camera_->GetCameraMatrix());
	EffectSystem::GetInstacne()->SetViewProjectionMatrix(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());

	// -------------------------------------------------
	// ↓ 行列の更新
	// -------------------------------------------------
	

#ifdef _DEBUG
	ImGuiDraw();
#endif
}

void TestScene::Draw() const {
	
}

#ifdef _DEBUG
void TestScene::ImGuiDraw() {
	ImGui::Begin("TestScene");
	
	ImGui::End();

}
#endif
