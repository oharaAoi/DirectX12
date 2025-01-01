#include "TestScene.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Editer/Window/EditerWindows.h"

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

	// worldObject -------------------------------------------------------------------
	skydome_ = std::make_unique<Skydome>();
	skydome_->Init();

	floor_ = std::make_unique<Floor>();
	floor_->Init();


	// gameObject -------------------------------------------------------------------
	testObjA_ = std::make_unique<TestObject>();
	testObjB_ = std::make_unique<TestObject>();

	testObjA_->Init();
	testObjB_->Init();

	testObjA_->SetCollider(1 << 1, ColliderShape::SPHERE);
	testObjB_->SetCollider(1 << 2, ColliderShape::OBB);

	testObjA_->SetObject("player.gltf");
	testObjA_->SetAnimater("./Engine/Resources/Animation/", "player.gltf", true, true, false);
	
	testObjB_->SetObject("amimationCharacter.gltf");
	testObjB_->SetAnimater("./Engine/Resources/Animation/", "amimationCharacter.gltf", true, true, false);

	testObjB_->GetTransform()->SetTranslaion(Vector3(2.0f, 0.0f, 0.0f));
	testObjB_->GetTransform()->SetQuaternion(Quaternion::AngleAxis(180.0f * toRadian, Vector3::UP()));
#ifdef _DEBUG
	EditerWindows::AddObjectWindow(std::bind(&TestObject::Debug_Gui, testObjA_.get()), "testAObj");
	EditerWindows::AddObjectWindow(std::bind(&TestObject::Debug_Gui, testObjB_.get()), "testAObj");
#endif

	//testObjB_->SetObject("skin.obj");

	// Manager -------------------------------------------------------------------

	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Init();

	/*EffectSystem::GetInstacne()->Emit("sphere", Vector3(0, 0, 5), Vector4(1, 0, 0, 1));
	EffectSystem::GetInstacne()->Emit("cone", Vector3(5, 0, 5), Vector4(0, 1, 0, 1));
	EffectSystem::GetInstacne()->Emit("box", Vector3(-5, 0, 5), Vector4(0, 0, 1, 1));*/
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
	// ↓ worldObjectの更新
	// -------------------------------------------------
	skydome_->Update();
	floor_->Update();

	// -------------------------------------------------
	// ↓ GameObjectの更新
	// -------------------------------------------------
	testObjA_->Update();
	testObjB_->Update();

	collisionManager_->Reset();
	collisionManager_->AddCollider(testObjA_->GetCollider());
	collisionManager_->AddCollider(testObjB_->GetCollider());
	collisionManager_->CheckAllCollision();

	// -------------------------------------------------
	// ↓ ParticleのViewを設定する
	// -------------------------------------------------

}

void TestScene::Draw() const {
	Engine::SetPipeline(PipelineType::NormalPipeline);
	skydome_->Draw();
	floor_->Draw();
	testObjA_->Draw();
	testObjB_->Draw();
}

#ifdef _DEBUG
void TestScene::Debug_Gui() {
	ImGui::Begin("TestScene");
	ImGui::Checkbox("isDebug", &isDebugCamera_);

	if (ImGui::Button("reserve")) {
		testObjA_->GetAnimetor()->GetAnimationClip()->SetAnimationReservation("slash", "attack2", raito_);
	}
	ImGui::DragFloat("raito", &raito_, 0.01f);

	ImGui::End();

	if (EffectSystem::GetInstacne()->GetIsEditerFocused()) {
		debugCamera_->SetIsFocused(false);
	} else {
		debugCamera_->SetIsFocused(true);
	}
}
#endif
