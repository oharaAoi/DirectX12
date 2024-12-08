#include "TestScene.h"
#include "Engine/Utilities/AdjustmentItem.h"

TestScene::TestScene() {}
TestScene::~TestScene() {}

void TestScene::Finalize() {
}

void TestScene::Init() {
	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	adjust->Init("TestScene");

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

	EffectSystem::GetInstacne()->Emit("sphere", Vector3(0, 0, 0), Vector4(1, 0, 0, 1));
	EffectSystem::GetInstacne()->Emit("cone", Vector3(5, 0, 0), Vector4(0, 1, 0, 1));
	EffectSystem::GetInstacne()->Emit("box", Vector3(-5, 0, 0), Vector4(0, 0, 1, 1));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 更新
//////////////////////////////////////////////////////////////////////////////////////////////////
void TestScene::Update() {
	// -------------------------------------------------
	// ↓ カメラの更新
	// -------------------------------------------------
	camera_->Update();
	if (isDebugCamera_) {
		debugCamera_->Update();
		Render::SetEyePos(debugCamera_->GetWorldTranslate());
		Render::SetViewProjection(debugCamera_->GetViewMatrix(), debugCamera_->GetProjectionMatrix());

		EffectSystem::GetInstacne()->SetCameraMatrix(debugCamera_->GetCameraMatrix());
		EffectSystem::GetInstacne()->SetViewProjectionMatrix(debugCamera_->GetViewMatrix(), debugCamera_->GetProjectionMatrix());
	} else {
		Render::SetEyePos(camera_->GetWorldTranslate());
		Render::SetViewProjection(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());
		Render::SetViewProjection2D(camera_->GetViewMatrix2D(), camera_->GetProjectionMatrix2D());

		EffectSystem::GetInstacne()->SetCameraMatrix(camera_->GetCameraMatrix());
		EffectSystem::GetInstacne()->SetViewProjectionMatrix(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());
	}

	// -------------------------------------------------
	// ↓ GameObjectの更新
	// -------------------------------------------------
	testObjA_->Update();
	testObjB_->Update();

	collisionManager_->Reset();
	collisionManager_->AddCollider(testObjA_->GetMeshCollider());
	collisionManager_->AddCollider(testObjB_->GetMeshCollider());
	collisionManager_->CheckAllCollision();

	// -------------------------------------------------
	// ↓ ParticleのViewを設定する
	// -------------------------------------------------

}

void TestScene::Draw() const {
	Engine::SetPipeline(PipelineType::NormalPipeline);
	/*testObjA_->Draw();

	testObjB_->Draw();*/

	Engine::SetPipeline(PipelineType::PrimitivePipeline);
}

#ifdef _DEBUG
void TestScene::Debug_Gui() {
	ImGui::Begin("TestScene");
	ImGui::Checkbox("isDebug", &isDebugCamera_);
	//test_animationCS_->Debug_Gui();
	testObjA_->Debug_Gui();
	camera_->Debug_Gui();
	debugCamera_->Debug_Gui();

	ImGui::Separator();

	ShowEasingDebug(easeIndex_);

	ImGui::End();

	if (EffectSystem::GetInstacne()->GetIsEditerFocused()) {
		debugCamera_->SetIsFocused(false);
	} else {
		debugCamera_->SetIsFocused(true);
	}
}
#endif
