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

	testObjA_->SetObject("simpleSkin.gltf");
	testObjA_->SetAnimater("./Engine/Resources/Animation/", "simpleSkin.gltf", true, true, false);
	testObjA_->GetTransform()->SetTranslaion(Vector3(2.0f, 0.0f, 0.0f));
	testObjA_->GetTransform()->SetQuaternion(Quaternion::AngleAxis(180.0f * toRadian, Vector3::UP()));

	testObjB_->SetObject("skin.obj");

	// Manager -------------------------------------------------------------------

	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Init();

	// Effect -------------------------------------------------------------------
	/*gpuEffect_ = std::make_unique<GpuEffect>();
	gpuEffect_->Init();*/
	Quaternion rotate0 = Quaternion::AngleAxis(0.45f, { 1.0f, 0.4f, -0.2f });
	Quaternion rotate1 = { -rotate0.x,  -rotate0.y, -rotate0.z, -rotate0.w };
	//Quaternion rotate1 = Quaternion::AngleAxis(PI, { 0.71f, 0.0f, 0.71f });

	/*Quaternion q1 = Quaternion::Slerp(rotate0, rotate1, 0.0f);
	Quaternion q2 = Quaternion::Slerp(rotate0, rotate1, 0.3f);
	Quaternion q3 = Quaternion::Slerp(rotate0, rotate1, 0.5f);
	Quaternion q4 = Quaternion::Slerp(rotate0, rotate1, 0.7f);
	Quaternion q5 = Quaternion::Slerp(rotate0, rotate1, 1.0f);*/

	Matrix4x4 mat = rotate0.MakeMatrix();

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

	gpuEffect_->Update();

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

	gpuEffect_->Draw();

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
