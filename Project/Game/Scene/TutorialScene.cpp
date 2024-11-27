#include "TutorialScene.h"

TutorialScene::TutorialScene() {}
TutorialScene::~TutorialScene() {}

void TutorialScene::Finalize() {

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialScene::Init() {
	adjustmentItem_ = AdjustmentItem::GetInstance();
	adjustmentItem_->Init("tutorialScene");

	// -------------------------------------------------
	// ↓ Cameraの初期化
	// -------------------------------------------------

	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Init();

	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Init();

	// -------------------------------------------------
	// ↓ worldObjectの初期化
	// -------------------------------------------------
	skydome_ = std::make_unique<Skydome>();
	skydome_->Init();

	field_ = std::make_unique<Field>();
	field_->Init();

	wall_ = std::make_unique<BaseGameObject>();
	wall_->Init();
	wall_->SetObject("wall.obj");

	// -------------------------------------------------
	// ↓ GameObjectの初期化
	// -------------------------------------------------
	player_ = std::make_unique<Player>();
	player_->Init();

	bossCore_ = std::make_unique<BossCore>();
	bossCore_->Init();
	bossCore_->SetBehaviorRequest(CoreState::Tutorial);

	leftSnaggeObj_ = std::make_unique<TestCollisionObj>();
	leftSnaggeObj_->Init();
	leftSnaggeObj_->SetPlayer(player_.get());
	leftSnaggeObj_->GetTransform()->SetTranslaion({ -6.0f,7.0f,0.0f });

	fall_ = std::make_unique<Fall>();
	fall_->Init();
	fall_->SetAppear(true);
	fall_->SetPlayer(player_.get());
	fall_->GetTransform()->SetTranslaion({ 10.0f, 20.0f, 12.0f });
	fall_->SetInitPosX(10.0f);
	
	fallStone_ = std::make_unique<FallStone>();
	fallStone_->Init();
	fallStone_->SetAppear(true);
	fallStone_->SetInitPosX(10.0f);

	// -------------------------------------------------
	// ↓ Managerの初期化
	// -------------------------------------------------
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Init();

	// -------------------------------------------------
	// ↓ 2d系の初期化
	// -------------------------------------------------

	panel_ = std::make_unique<Panel>();
	panel_->Init();

	tutorialUI_ = std::make_unique<TutorialUI>();
	tutorialUI_->Init();

	// -------------------------------------------------
	// ↓ ライト初期化
	// -------------------------------------------------
	SpotLight* spotLight = Render::GetSporLight();
	spotLight->AddAdjustment();
	spotLight->AdaptAdjustment();
	spotLight->Update();

	// -------------------------------------------------
	// ↓ 初期化時にやりたい処理を行う
	// -------------------------------------------------

	isDebugCamera_ = false;
	isNextScene_ = false;

	followCamera_->SetTarget(player_->GetTransform());
	Input::SetNotAccepted(false);
	panel_->SetBlackOutOpen();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialScene::Update() {

	skydome_->Update();
	field_->Update();
	wall_->Update();

	// -------------------------------------------------
	// ↓ GameObjectの更新
	// -------------------------------------------------

	if (isNextScene_) {
		AutoUpdate();
	} else {
		TutorialUpdate();
	}

	// -------------------------------------------------
	// ↓ GameObjectの更新
	// -------------------------------------------------

	panel_->Update();
	bossCore_->Update();
	leftSnaggeObj_->Update();


	if (fall_->GetTransform()->GetTranslation().y <= -10.0f) {
		fall_->Reset();
		fall_->SetAppear(true);
		fallStone_->Reset();
		fallStone_->SetAppear(true);
	}
	fall_->Update();
	fallStone_->SetFalling(fall_->GetFalling());
	fallStone_->Update();

	// -------------------------------------------------
	// ↓ Managerの更新
	// -------------------------------------------------

	collisionManager_->Reset();
	collisionManager_->AddCollider(player_->GetWireTipCollider());
	collisionManager_->AddCollider(leftSnaggeObj_.get());

	collisionManager_->AddCollider(player_->GetMeshCollider());
	collisionManager_->AddCollider(bossCore_->GetMeshCollider());
	if (!(!player_->GetIsStretchClutch() && !player_->GetIsReturnClutch())) {
		collisionManager_->AddCollider(player_->GetWireTip()->GetMeshCollider());
	}

	collisionManager_->CheckAllCollision();


	bool isFallNear_ = fall_->GetNear();
	bool isCoreNear_ = bossCore_->GetNear();
	if (!isCoreNear_) {
		isFallNear_ = fall_->CheckMouseNear(followCamera_->GetVpvpMatrix());
	}
	
	if (isFallNear_ || isCoreNear_) {
		player_->SetNearBack(true);
	}
	else {
		player_->SetNearBack(false);
	}


	tutorialUI_->Update(player_->GetWorldPos());

	// -------------------------------------------------
	// ↓ Cameraの更新
	// -------------------------------------------------
	debugCamera_->Update();

	if (player_->GetIsShake()) {
		followCamera_->SetShakeTime(0.5f);
	}
	followCamera_->Update();

	if (!isDebugCamera_) {
		Render::SetEyePos(followCamera_->GetWorldTranslate());
		Render::SetViewProjection(followCamera_->GetViewMatrix(), followCamera_->GetProjectionMatrix());
		Render::SetViewProjection2D(followCamera_->GetViewMatrix2D(), followCamera_->GetProjectionMatrix2D());
	} else {
		Render::SetEyePos(debugCamera_->GetWorldTranslate());
		Render::SetViewProjection(debugCamera_->GetViewMatrix(), debugCamera_->GetProjectionMatrix());
		Render::SetViewProjection2D(debugCamera_->GetViewMatrix2D(), debugCamera_->GetProjectionMatrix2D());
	}


	// -------------------------------------------------
	// ↓ Renderの更新
	// -------------------------------------------------
	if (!isDebugCamera_) {
		eyePos_ = followCamera_->GetWorldTranslate();
		viewMat_ = followCamera_->GetViewMatrix();
		projectionMat_ = followCamera_->GetProjectionMatrix();
	} else {
		eyePos_ = debugCamera_->GetWorldTranslate();
		viewMat_ = debugCamera_->GetViewMatrix();
		projectionMat_ = debugCamera_->GetProjectionMatrix();
	}

	Render::SetEyePos(eyePos_);
	Render::SetViewProjection(viewMat_, projectionMat_);
	Render::SetVpvpMat(followCamera_->GetVPVMatrix());
	Render::Update();

#ifdef _DEBUG
	Render::Debug_Gui();
	Debug_Gui();
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialScene::Draw() const {
	// -------------------------------------------------
	// ↓ worldObjectの描画
	// -------------------------------------------------
	Engine::SetPipeline(PipelineType::NormalPipeline);
	skydome_->Draw();

	field_->Draw();
	wall_->Draw();

	// -------------------------------------------------
	// ↓ GameObjectの描画
	// -------------------------------------------------

	player_->Draw();
	bossCore_->Draw();
	leftSnaggeObj_->Draw();
	fall_->Draw();
	fallStone_->Draw();

	tutorialUI_->Draw();

	// -------------------------------------------------
	// ↓ UIの描画
	// -------------------------------------------------
	Engine::SetPipeline(PipelineType::SpriteNormalBlendPipeline);
	panel_->Draw();

}

void TutorialScene::AutoUpdate() {
	if (panel_->GetIsFinished()) {
		nextSceneType_ = SceneType::GAME;
		return;
	}

	Vector3 playerPos = player_->GetTransform()->GetTranslation();
	if (playerPos.x >= 18.0f) {
		// ここでブラックアウトさせる
		if (panel_->GetDoNoting()) {
			panel_->SetBlackOut();
		}
	}
}

void TutorialScene::TutorialUpdate() {
	player_->SetInverMatrix(followCamera_->GetVPVMatrix().Inverse());
	player_->SetCameraZDis(followCamera_->GetTranslate().z);
	if (!isDebugCamera_) {
		player_->Update();
	}

	Vector3 playerPos = player_->GetTransform()->GetTranslation();
	if (playerPos.x >= 18.0f) {
		// ここでブラックアウトさせる
		isNextScene_ = true;
		Input::SetNotAccepted(true);
	}
}

void TutorialScene::AutoMove() {

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug編集
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void TutorialScene::Debug_Gui() {
	panel_->Debug_Gui();

	ImGui::Begin("TutorialScene");

	player_->Debug_Gui();

	if (ImGui::TreeNode("AdjustmentItem")) {
		// Updateだが実質Gui表示なためここで更新
		adjustmentItem_->Update();
		ImGui::TreePop();
	}

	tutorialUI_->Debug_Gui();

	ImGui::End();
}
#endif