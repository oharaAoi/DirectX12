#include "TestScene.h"

TestScene::TestScene() {}
TestScene::~TestScene() {}

void TestScene::Finalize() {
}

void TestScene::Init() {
	// カメラ -------------------------------------------------------------------
	camera_ = std::make_unique<Camera>();
	debugCamera_ = std::make_unique<DebugCamera>();

	// GameObject -------------------------------------------------------------------
	testObjA_ = std::make_unique<TestObject>();
	testObjB_ = std::make_unique<TestObject>();

	testObjA_->Init();
	testObjB_->Init();

	testObjA_->SetObject("amimationCharacter.gltf");
	testObjA_->SetAnimater("./Engine/Resources/Animation/", "amimationCharacter.gltf", true, true, false);
	testObjA_->GetTransform()->SetTranslaion(Vector3(2.0f, 0.0f, 0.0f));
	testObjA_->GetTransform()->SetQuaternion(Quaternion::AngleAxis(180.0f * toRadian, Vector3::UP()));

	testObjB_->SetObject("skin.obj");

	// Manager -------------------------------------------------------------------

	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Init();

	// Effect -------------------------------------------------------------------
	//gpuEffect_ = std::make_unique<GpuEffect>();
	//gpuEffect_->Init();
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
	testObjA_->Update();
	testObjB_->Update();

	//gpuEffect_->Update();

	collisionManager_->Reset();
	collisionManager_->AddCollider(testObjA_->GetMeshCollider());
	collisionManager_->AddCollider(testObjB_->GetMeshCollider());
	collisionManager_->CheckAllCollision();

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
	testObjA_->Draw();

	testObjB_->Draw();

	//gpuEffect_->Draw();

	Engine::SetPipeline(PipelineType::PrimitivePipeline);
	
}

#ifdef _DEBUG
void TestScene::ImGuiDraw() {
	ImGui::Begin("TestScene");
	ImGui::Checkbox("isDebug", &isDebugCamera_);
	//test_animationCS_->Debug_Gui();
	testObjA_->Debug_Gui();
	camera_->Debug_Gui();
	debugCamera_->Debug_Gui();

	ImGui::Separator();

	ShowEasingDebug(easeIndex_);

	ImGui::End();
}
#endif
