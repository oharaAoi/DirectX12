#include "TestScene.h"

TestScene::TestScene() {}
TestScene::~TestScene() {}

void TestScene::Init() {
	// カメラ -------------------------------------------------------------------
	camera_ = std::make_unique<Camera>();
	debugCamera_ = std::make_unique<DebugCamera>();
	/*sphereModel_ = Engine::CreateModel("sphere_Block.obj");*/
	
	testObj_ = std::make_unique<BaseGameObject>();
	testObj_->Init();
	testObj_->SetObject("walk.gltf");
	testObj_->SetAnimater("./Resources/Animation/", "sneakWalk.gltf");

	testObj2_ = std::make_unique<BaseGameObject>();
	testObj2_->Init();
	testObj2_->SetObject("skin.obj");

	testObj3_ = std::make_unique<BaseGameObject>();
	testObj3_->Init();
	testObj3_->SetObject("simpleSkin.gltf");
	testObj3_->SetAnimater("./Resources/Animation/", "simpleSkin.gltf");

	testObj2_->GetTransform().SetTranslation_X(2.0f);
	testObj3_->GetTransform().SetTranslation_X(-2.0f);

	//testObj2_->GetTransform().SetParent(testObj_->GetTransform().GetWorldMatrix());

	sprite_ = Engine::CreateSprite({128, 128}, {256, 256});
	sprite_->SetTexture("uvChecker.png");
}

void TestScene::Load() {
	// modelのload
	ModelManager::LoadModel("./Resources/Develop/", "plane.obj");
	ModelManager::LoadModel("./Resources/Develop/", "skin.obj");
	//ModelManager::LoadModel("./Resources/Develop/", "bunny.obj");
	ModelManager::LoadModel("./Resources/Develop/", "SquarePyramid.obj");
	ModelManager::LoadModel("./Resources/Animation/", "testSkinning.gltf");
	ModelManager::LoadModel("./Resources/Animation/", "walk.gltf");
	ModelManager::LoadModel("./Resources/Animation/", "simpleSkin.gltf");
	ModelManager::LoadModel("./Resources/Animation/", "sneakWalk.gltf");
	// textureのload
	TextureManager::LoadTextureFile("./Resources/Develop/", "uvChecker.png");
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
	testObj_->Update();
	testObj2_->Update();
	testObj3_->Update();
	
	sprite_->Update();

	Quaternion rotation0 = Quaternion::AngleAxis(0.3f, Normalize({ 0.71f, 0.71f, 0.0f }));
	Quaternion rotation1 = Quaternion::AngleAxis(3.141592f, { 0.71f, 0.0f, 0.71f });
	Quaternion rotation2 = Quaternion::AngleAxis(0.45f, Normalize({ 1.0f, 0.4f, -0.2f }));
	Quaternion result = Quaternion::Slerp(rotation0, rotation1, 0.3f);

	ImGui::Begin("Quaternion");
	ImGui::DragFloat4("Qua", &rotation2.x, 0.01f);
	ImGui::End();

	// -------------------------------------------------
	// ↓ の更新
	// -------------------------------------------------
#ifdef _DEBUG
	ImGuiDraw();
#endif
}

void TestScene::Draw() const {
	//DrawGrid(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());

#pragma region NormalPipeline

	Engine::SetPipeline(PipelineKind::kNormalPipeline);
	//testObj_->Draw();
	testObj2_->Draw();
	
#pragma endregion

#pragma region Textureless

	Engine::SetPipeline(PipelineKind::kTexturelessPipeline);

#pragma endregion

#pragma region PBR

	Engine::SetPipeline(PipelineKind::kPBRPipeline);

#pragma endregion

#pragma region Sprite

	Engine::SetPipeline(PipelineKind::kSpritePipeline);
	sprite_->Draw();
	
#pragma endregion

	Engine::SetPipeline(PipelineKind::kSkinningPipeline);
	testObj_->Draw();
	testObj3_->Draw();
}

#ifdef _DEBUG
void TestScene::ImGuiDraw() {
	ImGui::Begin("GameObjects");
	testObj_->Debug_Gui();
	testObj3_->Debug_Gui();
	ImGui::End();

	ImGui::Begin("Sprite");
	sprite_->Debug_Gui();
	ImGui::End();

}
#endif
