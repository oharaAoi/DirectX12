#include "GameScene.h"

GameScene::GameScene() {
}

GameScene::~GameScene() {
}

void GameScene::Init() {
	camera_ = std::make_unique<Camera>();

	sphere_ = Engine::CreateSphere(16);

	roughness_ = 0.5f;
	metallic_ = 0.5f;

	lightKind_ = 0;

	soundData_ = Engine::LoadSE("Resources/Audio/fanfare.wav");
	bgmData_ = Engine::LoadBGM("Resources/Audio/fanfare.wav");
}

void GameScene::Load() {
}

void GameScene::Update() {
	//transform_.rotate.y += 0.01f;

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// カメラの更新
	//////////////////////////////////////////////////////////////////////////////////////////////////
	camera_->Update();
	Render::SetEyePos(camera_->GetWorldTranslate());
	Render::SetViewProjection(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// sound
	//////////////////////////////////////////////////////////////////////////////////////////////////
	if (Input::IsTriggerKey(DIK_P)) {
		Engine::PlaySE(soundData_, false);
	}

	if (Input::IsTriggerKey(DIK_O)) {
		Engine::PlayBGM(bgmData_, true);
	}

	if (Input::IsTriggerKey(DIK_S)) {
		Engine::StopBGM(bgmData_);
	}

	if (Input::IsTriggerKey(DIK_Q)) {
		Engine::PauseBGM(bgmData_);
	}

	if (Input::IsTriggerKey(DIK_W)) {
		Engine::ReStartBGM(bgmData_);
	}

	Vector2 joyStickLeft = Input::GetLeftJoyStick();
	Vector2 joyStickRight = Input::GetRightJoyStick();
	ImGui::Begin("Input");
	ImGui::Text("LX : %f  LY : %f", joyStickLeft.x, joyStickLeft.y);
	ImGui::Text("RX : %f  RY : %f", joyStickRight.x, joyStickRight.y);
	if (Input::IsTriggerKey(DIK_SPACE)) {
		ImGui::Text("TriggerSpace");
	}

	if (Input::IsPressKey(DIK_SPACE)) {
		ImGui::Text("PressSpace");
	}

	ImGui::End();

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// ImGui
	//////////////////////////////////////////////////////////////////////////////////////////////////
	ImGui::Begin("Setting");

	if (ImGui::CollapsingHeader("materials")) {
		ImGui::DragFloat("metallic", &metallic_, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("roughness", &roughness_, 0.01f, 0.0f, 1.0f);
	}

	ImGui::End();

	/*model_->ImGuiDraw("floor");
	sphere_->ImGuiDraw("Sphere");
	sphereModel_->ImGuiDraw("sphereModel");
	teapotModel_->ImGuiDraw("teapot");*/
}

void GameScene::Draw() const {
#pragma region NormalPipeline

	Engine::SetPipeline(PipelineKind::kNormalPipeline);

#pragma endregion

#pragma region Textureless

	Engine::SetPipeline(PipelineKind::kTexturelessPipeline);

#pragma endregion

#pragma region PBR

	Engine::SetPipeline(PipelineKind::kPBRPipeline);
	//Engine::DrawModel(sphereModel_.get());


#pragma endregion

#pragma region Particle

	Engine::SetPipeline(PipelineKind::kParticlePipeline);
	

#pragma endregion

	
}
