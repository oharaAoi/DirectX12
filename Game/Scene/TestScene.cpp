#include "TestScene.h"

TestScene::TestScene() {}
TestScene::~TestScene() {}

void TestScene::Init() {
	// カメラ -------------------------------------------------------------------
	camera_ = std::make_unique<Camera>();
	/*sphereModel_ = Engine::CreateModel("sphere_Block.obj");*/
	
	testObj_ = std::make_unique<BaseGameObject>();
	testObj_->Init();
	testObj_->SetObject("walk.gltf");
	testObj_->SetAnimater("./Resources/Animation/", "walk.gltf");

	sprite_ = Engine::CreateSprite({128, 128}, {256, 256});
	sprite_->SetTexture("uvChecker.png");
}

void TestScene::Load() {
	// modelのload
	ModelManager::LoadModel("./Resources/Develop/", "plane.obj");
	ModelManager::LoadModel("./Resources/Develop/", "SquarePyramid.obj");
	ModelManager::LoadModel("./Resources/Animation/", "walk.gltf");
	ModelManager::LoadModel("./Resources/Animation/", "simpleSkin.gltf");
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
	
	sprite_->Update();

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
	testObj_->DrawSKinning();
}

#ifdef _DEBUG
void TestScene::ImGuiDraw() {
	ImGui::Begin("GameObjects");
	testObj_->Debug_Gui();
	ImGui::End();

	ImGui::Begin("Sprite");
	sprite_->Debug_Gui();
	ImGui::End();

}
#endif
