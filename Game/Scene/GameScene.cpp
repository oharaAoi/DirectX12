#include "GameScene.h"

GameScene::GameScene() {
}

GameScene::~GameScene() {
}

void GameScene::Init() {
	camera_ = std::make_unique<Camera>();

	sphere_ = Engine::CreateSphere(16);

	/*model_ = Engine::CreateModel("floor.obj");*/
	sphereModel_ = Engine::CreateModel("sphere.obj");
	teapotModel_ = Engine::CreateModel("plane.obj");

	terrainModel_ = Engine::CreateModel("terrain.obj");

	particleModel_ = Engine::CreateModel("particle.obj");

	sphereTransform_ = Engine::CreateWorldTransform();
	sphereTransform_ = Engine::CreateWorldTransform();
	sphereModelTransform_ = Engine::CreateWorldTransform();

	particle_ = std::make_unique<Particle>();
	particle_->Init("particle.obj", 100);

	emitter_ = std::make_unique<Emitter>();
	emitter_->Init();

	particleField_ = std::make_unique<ParticleField>();
	particleField_->Init();

	emitter_->SetParticle(particle_.get());
	particleField_->SetParticle(particle_.get());

	roughness_ = 0.5f;
	metallic_ = 0.5f;

	lightKind_ = 0;

	soundData_ = Engine::LoadSE("Resources/fanfare.wav");
	bgmData_ = Engine::LoadBGM("Resources/fanfare.wav");
}

void GameScene::Update() {
	//transform_.rotate.y += 0.01f;

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// カメラの更新
	//////////////////////////////////////////////////////////////////////////////////////////////////
	camera_->Update();
	Engine::SetEyePos(camera_->GetWorldTranslate());
	Engine::SetViewProjection(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());

	sphereTransform_.Update();
	sphereModelTransform_.Update();

	emitter_->Update();

	particle_->SetCameraMatrix(camera_->GetCameraMatrix());
	particle_->Update(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 当たり判定
	//////////////////////////////////////////////////////////////////////////////////////////////////
	particleField_->Update();

	Engine::SetEyePos(camera_->GetWorldTranslate());
	
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
		sphere_->SetMaterials(roughness_, metallic_);
		model_->SetMaterials(roughness_, metallic_);
		sphereModel_->SetMaterials(roughness_, metallic_);
		teapotModel_->SetMaterials(roughness_, metallic_);

		ImGui::DragFloat("metallic", &metallic_, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("roughness", &roughness_, 0.01f, 0.0f, 1.0f);
	}

	if (ImGui::CollapsingHeader("LightingKind")) {
		ImGui::Combo("lightKind", &lightKind_, "directional\0PBR");

		LightGroup::LightKind kind = static_cast<LightGroup::LightKind>(lightKind_);
		Engine::SetLightKind(kind);
	}

	ImGui::End();

	/*model_->ImGuiDraw("floor");
	sphere_->ImGuiDraw("Sphere");
	sphereModel_->ImGuiDraw("sphereModel");
	teapotModel_->ImGuiDraw("teapot");*/

	particle_->ImGuiDraw();

	sphereModel_->ImGuiDraw("sphereModel");
}

void GameScene::Draw() {
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
	//particle_->Draw();

#pragma endregion

	emitter_->Draw(camera_->GetViewMatrix() * camera_->GetProjectionMatrix());
	particleField_->Draw(camera_->GetViewMatrix() * camera_->GetProjectionMatrix());
}
