#include "TestScene.h"
#include "Engine.h"
#include "Engine/System/Input/Input.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Editer/Window/EditerWindows.h"
#include "Engine/System/ParticleSystem/EffectSystem.h"

TestScene::TestScene() {}
TestScene::~TestScene() {}

void TestScene::Finalize() {
}

void TestScene::Init() {
	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	adjust->Init("TestScene");

	// カメラ -------------------------------------------------------------------
	camera2d_ = std::make_unique<Camera2d>();
	camera3d_ = std::make_unique<Camera3d>();
	debugCamera_ = std::make_unique<DebugCamera>();

	camera2d_->Init();
	camera3d_->Init();
	debugCamera_->Init();

	// worldObject -------------------------------------------------------------------
	skydome_ = std::make_unique<Skydome>();
	skydome_->Init();

	floor_ = std::make_unique<Floor>();
	floor_->Init();

	// gameObject -------------------------------------------------------------------
	testObjA_ = std::make_unique<TestObject>();

	testObjA_->Init();
	testObjA_->SetCollider(1 << 1, ColliderShape::SPHERE);
	
#ifdef _DEBUG
	EditerWindows::AddObjectWindow(std::bind(&TestObject::Debug_Gui, testObjA_.get()), "testAObj");
#endif

	//testObjB_->SetObject("skin.obj");

	// Manager -------------------------------------------------------------------

	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Init();

	//EffectSystem::GetInstacne()->Emit("test", Vector3(0, 0, 0));

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
	camera2d_->Update();
	camera3d_->Update();
	if (isDebugCamera_) {
		debugCamera_->Update();
		
		EffectSystem::GetInstacne()->SetCameraMatrix(debugCamera_->GetCameraMatrix());
		EffectSystem::GetInstacne()->SetViewProjectionMatrix(debugCamera_->GetViewMatrix(), debugCamera_->GetProjectionMatrix());
	} else {
		
		EffectSystem::GetInstacne()->SetCameraMatrix(camera3d_->GetCameraMatrix());
		EffectSystem::GetInstacne()->SetViewProjectionMatrix(camera3d_->GetViewMatrix(), camera3d_->GetProjectionMatrix());
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
	
	collisionManager_->Reset();
	collisionManager_->AddCollider(testObjA_->GetCollider());
	collisionManager_->CheckAllCollision();

	// -------------------------------------------------
	// ↓ ParticleのViewを設定する
	// -------------------------------------------------

}

void TestScene::Draw() const {
	if (isDebugCamera_) {
		DrawGrid(debugCamera_->GetViewMatrix(), debugCamera_->GetProjectionMatrix());
	} else {
		DrawGrid(camera3d_->GetViewMatrix(), camera3d_->GetProjectionMatrix());
	}

	Engine::SetPipeline(PipelineType::NormalPipeline);
	skydome_->Draw();
	floor_->Draw();
	testObjA_->Draw();
}

#ifdef _DEBUG
void TestScene::Debug_Gui() {
	ImGui::Checkbox("isDebug", &isDebugCamera_);

	if (ImGui::Button("reserve")) {
		testObjA_->GetAnimetor()->GetAnimationClip()->ReservationAnimation("slash", "attack2", 0.5f, raito_);
	}
	ImGui::DragFloat("raito", &raito_, 0.01f);
}
#endif
