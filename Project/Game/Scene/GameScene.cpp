#include "GameScene.h"

GameScene::GameScene() {
}

GameScene::~GameScene() {
}

void GameScene::Finalize() {
	gameObjectManager_->Finalize();
}

void GameScene::Init() {
	adjustmentItem_ = AdjustmentItem::GetInstance();
	adjustmentItem_->Init("gameScene");

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

	backGround_ = std::make_unique<BackGround>();
	backGround_->Init();

	field_ = std::make_unique<Field>();
	field_->Init();

	// -------------------------------------------------
	// ↓ GameObjectの初期化
	// -------------------------------------------------
	player_ = std::make_unique<Player>();
	player_->Init();

	boss_ = std::make_unique<Boss>();
	boss_->Init();

	rightSnaggeObj_ = std::make_unique<TestCollisionObj>();
	rightSnaggeObj_->Init();
	rightSnaggeObj_->SetPlayer(player_.get());

	leftSnaggeObj_ = std::make_unique<TestCollisionObj>();
	leftSnaggeObj_->Init();
	leftSnaggeObj_->SetPlayer(player_.get());
	leftSnaggeObj_->GetTransform()->SetTranslaion({ -8.0f,7.0f,0.0f });


	fall_ = std::make_unique<Fall>();
	fall_->Init();
	fall_->SetPlayer(player_.get());

	fallStone_ = std::make_unique<FallStone>();
	fallStone_->Init();

	// -------------------------------------------------
	// ↓ Managerの初期化
	// -------------------------------------------------

	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Init();

	gameObjectManager_ = GameObjectManager::GetInstance();
	gameObjectManager_->Init();

	// -------------------------------------------------
	// ↓ Editer初期化
	// -------------------------------------------------
	bossLeftAttackEditer_ = std::make_unique<BossAttackEditer>();
	bossLeftAttackEditer_->Init();

	bossRightAttackEditer_ = std::make_unique<BossAttackEditer>();
	bossRightAttackEditer_->Init();

	// -------------------------------------------------
	// ↓ UI初期化
	// -------------------------------------------------
	playerUI_ = std::make_unique<PlayerUI>();
	playerUI_->Init(player_.get());

	bossUI_ = std::make_unique<BossUI>(boss_.get());
	bossUI_->Init();

	panel_ = std::make_unique<Panel>();
	panel_->Init();
	panel_->SetBlackOutOpen(2.5f);

	bossHpUI_ = std::make_unique<BossHpUI>(boss_.get());
	bossHpUI_->Init();

	// -------------------------------------------------
	// ↓ ライト初期化
	// -------------------------------------------------
	SpotLight* spotLight = Render::GetSporLight();
	spotLight->AddAdjustment();
	spotLight->AdaptAdjustment();

	// -------------------------------------------------
	// ↓ 初期化時にやりたい処理を行う
	// -------------------------------------------------

	isPlayerAutoMove_ = true;
	finishAppear_ = false;
	Input::SetNotAccepted(false);

	isDebugCamera_ = false;
	followCamera_->SetTarget(player_->GetTransform());

	boss_->SetEditer(bossLeftAttackEditer_.get(), bossRightAttackEditer_.get());
	boss_->SetGameScene(this);

}

void GameScene::Update() {
	if (!player_->GetIsAlive()) {
		nextSceneType_ = SceneType::GAMEOVER;
		return;
	}

	if (!boss_->GetIsAlive()) {
		nextSceneType_ = SceneType::GAMECLEAR;
		return;
	}

	if (!finishAppear_) {
		AppearUpdate();
	}

	// -------------------------------------------------
	// ↓ worldObjectの更新
	// -------------------------------------------------
	UpdateWorldObject();

	// -------------------------------------------------
	// ↓ GameObjectの更新
	// -------------------------------------------------
	if (!boss_->GetIsTransitionForm()) {
		UpdateGameObject();
	} else {
		BossFormTransition();
	}

	// -------------------------------------------------
	// ↓ UIの更新
	// -------------------------------------------------
	UpdateUI();

	// -------------------------------------------------
	// ↓ Managerの更新
	// -------------------------------------------------
	UpdateManager();

	if (!finishAppear_) {
		player_->SetNearBack(false);
	}

	// -------------------------------------------------
	// ↓ Cameraの更新
	// -------------------------------------------------

	debugCamera_->Update();
	if (player_->GetIsShake()) {
		followCamera_->SetShakeTime(0.3f);
	}
	if ((boss_->GetBossLeftHand()->GetIsGroundSlap() || boss_->GetBossRightHand()->GetIsGroundSlap())) {
		followCamera_->SetShakeTime(0.8f);
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

#ifdef _DEBUG
	Render::Debug_Gui();
	Debug_Gui();
#endif
}

void GameScene::Draw() const {
	// -------------------------------------------------
	// ↓ Debugの描画
	// -------------------------------------------------
#ifdef _DEBUG
	Engine::SetPipeline(PipelineType::PrimitivePipeline);
	//DrawGrid(viewMat_, projectionMat_);

	field_->Debug_Draw();
	bossLeftAttackEditer_->Draw();
	bossRightAttackEditer_->Draw();

	player_->Debug_Draw();
	boss_->Debug_Draw();

	for (auto& missile : missileList_) {
		missile->Debug_Draw();
	}

	gameObjectManager_->Debug_Draw();

#endif

	// -------------------------------------------------
	// ↓ worldObjectの描画
	// -------------------------------------------------
	Engine::SetPipeline(PipelineType::NormalPipeline);
	skydome_->Draw();
	backGround_->Draw();
	field_->Draw();

	// -------------------------------------------------
	// ↓ GameObjectの描画
	// -------------------------------------------------
	boss_->Draw();

	Engine::SetPipeline(PipelineType::NormalPipeline);

	bool isPullSign = false;
	if (boss_->GetBossForm() == BossForm::SECOND) {
		isPullSign = (!(boss_->GetBossCore()->GetFalling()) && (boss_->GetBossBarrier()->GetEnableFunction() && boss_->GetBossBarrier()->GetIsBreak()));
	} else if (boss_->GetBossForm() == BossForm::FIRST) {
		isPullSign = !(boss_->GetBossCore()->GetFalling());
	}

	Engine::SetPipeline(PipelineType::NormalPipeline);
	for (auto& missile : missileList_) {
		missile->Draw();
	}

	Engine::SetPipeline(PipelineType::NormalPipeline);
	player_->Draw();

	gameObjectManager_->Draw();

	rightSnaggeObj_->Draw();
	leftSnaggeObj_->Draw();

	fall_->Draw();
	fallStone_->Draw();

	boss_->PostDraw();

	// -------------------------------------------------
	// ↓ UIの描画
	// -------------------------------------------------
	Engine::SetPipeline(PipelineType::SpriteNormalBlendPipeline);
	bossUI_->Draw();

	boss_->DrawUI();

	for (const auto& missile : missileList_) {
		missile->DrawUI();
	}

	fall_->DrawUI();

	panel_->Draw();

	// -------------------------------------------------
	// ↓ 3dUIの描画
	// -------------------------------------------------
	Engine::SetPipeline(PipelineType::NormalPipeline);
	bossUI_->Draw3dObject(player_->GetCanBossAttack(), isPullSign);
	fall_->DrawUI3D();

	Engine::ClearRenderTarget();
	Engine::SetPipeline(PipelineType::NormalPipeline);
	if (finishAppear_) {
		bossHpUI_->Draw();
	}

	for (const auto& missile : missileList_) {
		missile->DrawReticle();
	}

	playerUI_->Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　worldObjectの更新
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::UpdateWorldObject() {
	skydome_->Update();
	backGround_->Update();
	field_->Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　GameObjectの更新
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::UpdateGameObject() {
	boss_->SetPlayerPos(player_->GetWorldPos());
	boss_->GetBossCore()->SetPlayerPullBack(player_->GetPullBack());
	boss_->Update();
	if (boss_->GetBossCore()->SetFalsePlayerPullBack()) {
		player_->SetFalsePullBack();
	}

	// playerの処理
	player_->SetInverMatrix(followCamera_->GetVPVMatrix().Inverse());
	player_->SetCameraZDis(followCamera_->GetTranslate().z);

	player_->CheckBossToLength(boss_->GetBossBodyPos());
	if (!isDebugCamera_) {
		player_->Update();
	}

	gameObjectManager_->SetBombPara(player_->GetWireTip()->GetNeglect(), player_->GetPull(), player_->GetWireTip()->GetTransform()->GetTranslation());
	gameObjectManager_->Update();

	// missileの処理
	missileList_.remove_if([](auto& enemy) {return !enemy->GetIsAlive(); });
	for (auto& missile : missileList_) {
		missile->SetCameraMat(followCamera_->GetCameraMatrix());
		missile->Update();
	}

	rightSnaggeObj_->Update();
	leftSnaggeObj_->Update();

	if (boss_->GetBossBarrier()->GetIsExpand()) {
		fall_->Reset();
		fallStone_->Reset();
	}
	if (boss_->GetBossForm() == BossForm::SECOND && (boss_->GetBossLeftHand()->GetIsGroundSlap() || boss_->GetBossRightHand()->GetIsGroundSlap())) {
		fall_->SetAppear(true);
	}

	fall_->Update();
	fallStone_->SetFalling(fall_->GetFalling());
	fallStone_->Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　UIの更新
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::UpdateUI() {
	playerUI_->SetPlayerScreenPos(player_->GetTransform()->GetWorldMatrix(), followCamera_->GetVpvpMatrix());
	playerUI_->Update(player_->GetHp(), player_->GetHpLimit());

	bossUI_->Update(boss_->GetBossHp(), followCamera_->GetVpvpMatrix());

	if (finishAppear_) {
		bossHpUI_->Update(boss_->GetBossHp());
	}

	for (auto& missile : missileList_) {
		missile->UpdateUI(followCamera_->GetVpvpMatrix());
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Managerの更新
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::UpdateManager() {
	collisionManager_->Reset();
	collisionManager_->AddCollider(player_->GetWireTipCollider());
	collisionManager_->AddCollider(rightSnaggeObj_.get());
	collisionManager_->AddCollider(leftSnaggeObj_.get());

	// mesh
	collisionManager_->AddCollider(boss_->GetBossCore()->GetMeshCollider());
	collisionManager_->AddCollider(boss_->GetBossBody()->GetMeshCollider());
	/*boss_->GetBossRightHand()->SetIsGroundSlap(false);
	boss_->GetBossLeftHand()->SetIsGroundSlap(false);*/
	boss_->GetBossCore()->SetPlayerState(player_->GetPlayerState());

	if (boss_->GetBossRightHand()->GetIsAlive()) {
		collisionManager_->AddCollider(boss_->GetBossRightHand()->GetMeshCollider());
	}
	if (boss_->GetBossLeftHand()->GetIsAlive()) {
		collisionManager_->AddCollider(boss_->GetBossLeftHand()->GetMeshCollider());
	}
	collisionManager_->AddCollider(field_->GetMeshCollider());
	collisionManager_->AddCollider(player_->GetMeshCollider());
	collisionManager_->AddCollider(fallStone_->GetMeshCollider());

	if (!(!player_->GetIsStretchClutch() && !player_->GetIsReturnClutch())) {
		collisionManager_->AddCollider(player_->GetWireTip()->GetMeshCollider());
	}

	// バリアが機能していたらコリジョンのリストに追加
	if (boss_->GetBossBarrier()->GetEnableFunction()) {
		collisionManager_->AddCollider(boss_->GetBossBarrier()->GetMeshCollider());
	}

	// ミサイルを追加
	for (auto& missile : missileList_) {
		collisionManager_->AddCollider(missile->GetMeshCollider());
	}

	// ボムの追加
	for (auto& bomb : gameObjectManager_->GetBombList()) {
		collisionManager_->AddCollider(bomb->GetMeshCollider());
	}

	collisionManager_->CheckAllCollision();

	bool isFallNear_ = fall_->GetNear();
	bool isCoreNear_ = boss_->GetBossCore()->GetNear();
	if (!isCoreNear_) {
		isFallNear_ = fall_->CheckMouseNear(followCamera_->GetVpvpMatrix());
	}
	if (!isFallNear_) {
		isCoreNear_ = boss_->GetBossCore()->CheckMouseNear(followCamera_->GetVpvpMatrix());
	}
	if (boss_->GetBossBarrier()->GetEnableFunction() && !boss_->GetBossBarrier()->GetIsBreak()) {
		boss_->GetBossCore()->SetNear(false);
		isCoreNear_ = false;
	}


	if (isFallNear_ || isCoreNear_) {
		player_->SetNearBack(true);
	} else {
		player_->SetNearBack(false);
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　ボスの状態を遷移させる
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::BossFormTransition() {
	bossFormTransitionTime_ += GameTimer::DeltaTime();

	boss_->Update();

	if (bossFormTransitionTime_ <= 1.0f) {
		boss_->GetBossCore()->SetIsAppearReset(true);
	}
	if (bossFormTransitionTime_ >= bossFormTransitionTimeLimit_) {
		boss_->SetIsTransitionForm(false);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　ゲーム開始時の更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::AppearUpdate() {
	panel_->Update();

	// 自動で動かす
	if (isPlayerAutoMove_) {
		if (player_->GetTransform()->GetTranslation().x <= -17.0f) {
			player_->AutoMove(Vector3{ 4.0f, 0.0f, 0.0f });
		} else {
			isPlayerAutoMove_ = false;
		}
	}

	if (boss_->GetIsAppear()) {
		finishAppear_ = true;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　missileの追加
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::AddMissile(const Vector3& targePos, const Vector3& firePos) {
	auto& newMissile = missileList_.emplace_back(std::make_unique<Missile>());
	newMissile->Init();
	newMissile->Pop(targePos, firePos);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void GameScene::Debug_Gui() {
	ImGui::Begin("GameScene");
	{
		if (ImGui::TreeNode("Camera")) {
			ImGui::Checkbox("isDebug", &isDebugCamera_);

			debugCamera_->Debug_Gui();
			followCamera_->Debug_Gui();
			ImGui::TreePop();
		}

		if (Input::IsTriggerKey(DIK_C)) {
			isDebugCamera_ = !isDebugCamera_;
		}
	}

	{
		if (ImGui::TreeNode("GameObject")) {
			player_->Debug_Gui();
			boss_->Debug_Gui();
			fall_->Debug_Gui();
			ImGui::TreePop();
		}
	}

	{
		if (ImGui::TreeNode("WorldObject")) {
			field_->Debug_Gui();
			ImGui::TreePop();
		}
	}

	{
		if (ImGui::TreeNode("UI")) {
			bossUI_->Debug_Gui();
			bossHpUI_->Debug_Gui();
			playerUI_->Debug_Gui();
			ImGui::TreePop();
		}
	}

	{
		if (ImGui::TreeNode("Manager")) {

			ImGui::TreePop();
		}
	}

	{
		if (ImGui::TreeNode("AdjustmentItem")) {
			// Updateだが実質Gui表示なためここで更新
			adjustmentItem_->Update();
			ImGui::TreePop();
		}
	}

	boss_->CheckMouseCursolCollision(debugCamera_->GetVpvpMatrix());
	bossLeftAttackEditer_->Update();
	bossRightAttackEditer_->Update();
	{
		if (ImGui::TreeNode("BossAttackEditer")) {
			ImGui::Begin("BossAttackEditer");
			if (ImGui::TreeNode("Left")) {
				bossRightAttackEditer_->AddPoint();
				bossRightAttackEditer_->DeletePoint(debugCamera_->GetVpvpMatrix());
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Right")) {
				bossRightAttackEditer_->AddPoint();
				bossRightAttackEditer_->DeletePoint(debugCamera_->GetVpvpMatrix());
				ImGui::TreePop();
			}
			ImGui::End();
			ImGui::TreePop();
		}
	}

	if (ImGui::Button("Fire")) {
		AddMissile(player_->GetWorldPos(), boss_->GetBossBodyPos());
	}

	ImGui::SliderFloat("transitionTime", &bossFormTransitionTime_, 0.0f, bossFormTransitionTimeLimit_);
	ImGui::SliderFloat("transitionTimeLmit", &bossFormTransitionTimeLimit_, 0.0f, 30.0f);

	ImGui::Separator();
	ImGui::DragFloat3("bombPopPos", &bombPopPos_.x, 0.1f);
	if (ImGui::Button("popBomb")) {
		gameObjectManager_->PopBomb(bombPopPos_);
	}

	ImGui::Checkbox("finishAppear", &finishAppear_);

	panel_->Debug_Gui();

	backGround_->Debug_Gui();

	ImGui::End();
}

#endif