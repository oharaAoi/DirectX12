#include "TestScene.h"

TestScene::TestScene() { Init(); }
TestScene::~TestScene() {}

void TestScene::Init() {
	// カメラ -------------------------------------------------------------------
	camera_ = std::make_unique<Camera>();
	/*sphereModel_ = Engine::CreateModel("sphere_Block.obj");*/
	
	testObj_ = std::make_unique<BaseGameObject>();
	testObj_->Init();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 更新
//////////////////////////////////////////////////////////////////////////////////////////////////
void TestScene::Update() {
	// -------------------------------------------------
	// ↓ カメラの更新
	// -------------------------------------------------
	camera_->Update();
	Render::SetEyePos(camera_->GetWorldTranslate());
	Render::SetViewProjection(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());
	Render::SetViewProjection2D(camera_->GetViewMatrix2D(), camera_->GetProjectionMatrix2D());

	// -------------------------------------------------
	// ↓ ParticleのViewを設定する
	// -------------------------------------------------
	EffectSystem::GetInstacne()->SetCameraMatrix(camera_->GetCameraMatrix());
	EffectSystem::GetInstacne()->SetViewProjectionMatrix(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());

	// -------------------------------------------------
	// ↓ 行列の更新
	// -------------------------------------------------
	testObj_->Update();

	// -------------------------------------------------
	// ↓ の更新
	// -------------------------------------------------
	ImGuiDraw();
}

void TestScene::Draw() const {
	//DrawGrid(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());

#pragma region NormalPipeline

	Engine::SetPipeline(PipelineKind::kNormalPipeline);
	//Engine::DrawModel(cubeModel_.get(), cubeTransform_);

	testObj_->Draw();

#pragma endregion

#pragma region Textureless

	Engine::SetPipeline(PipelineKind::kTexturelessPipeline);

#pragma endregion

#pragma region PBR

	Engine::SetPipeline(PipelineKind::kPBRPipeline);

#pragma endregion
}

#ifdef _DEBUG
void TestScene::ImGuiDraw() {
	ImGui::Begin("GameObjects");
	testObj_->Debug_Gui();
	ImGui::End();
}
#endif
