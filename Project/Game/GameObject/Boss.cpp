#include "Boss.h"
#include "Game/Scene/GameScene.h"

Boss::Boss() {
}

Boss::~Boss() {
}

void Boss::Finalize() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Boss::Init() {
	boss_body_ = std::make_unique<BossBody>();
	boss_body_->Init();

	boss_core_ = std::make_unique<BossCore>();
	boss_core_->Init();

	boss_leftHand_ = std::make_unique<BossLeftHand>();
	boss_leftHand_->Init();

	boss_rightHand_ = std::make_unique<BossRightHand>();
	boss_rightHand_->Init();

	boss_barrier_ = std::make_unique<BossBarrier>();
	boss_barrier_->Init();

	state_ = std::make_unique<BossRootState>(this);

	// -------------------------------------------------
	// ↓ handとbodyをペアレントさせる
	// -------------------------------------------------

	boss_core_->GetTransform()->SetParent(boss_body_->GetTransform()->GetWorldMatrix());
	boss_leftHand_->GetTransform()->SetParent(boss_body_->GetTransform()->GetWorldMatrix());
	boss_rightHand_->GetTransform()->SetParent(boss_body_->GetTransform()->GetWorldMatrix());
	boss_barrier_->GetTransform()->SetParent(boss_body_->GetTransform()->GetWorldMatrix());

	// -------------------------------------------------
	// ↓ 攻撃の情報が入ったフォルダを作成
	// -------------------------------------------------

	std::filesystem::path dire(attackDirectoryPath_);
	if (!std::filesystem::exists(attackDirectoryPath_)) {
		std::filesystem::create_directories(attackDirectoryPath_);
	}

	behaviorRequest_ = Behavior::ROOT;
	CheckBehaviorRequest();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Boss::Update() {
	CheckBehaviorRequest();
	state_->Update();

	// -------------------------------------------------
	// ↓ playerの座標から右手と左手でどちらが近いかを計算しておく
	// -------------------------------------------------

	float playerToLeft = (playerPos_ - boss_leftHand_->GetTransform()->GetTranslation()).Length();
	float playerToRight = (playerPos_ - boss_rightHand_->GetTransform()->GetTranslation()).Length();

	if (playerToLeft < playerToRight) {
		boss_leftHand_->SetIsNear(true);
		boss_rightHand_->SetIsNear(false);
		near_ = "left";
	} else {
		boss_leftHand_->SetIsNear(false);
		boss_rightHand_->SetIsNear(true);
		near_ = "right";
	}

	// -------------------------------------------------
	// ↓ 行列の更新
	// -------------------------------------------------

	boss_leftHand_->SetBodyPos(boss_body_->GetTransform()->GetTranslation());
	boss_rightHand_->SetBodyPos(boss_body_->GetTransform()->GetTranslation());

	boss_body_->Update();
	boss_core_->Update();
	boss_leftHand_->Update();
	boss_rightHand_->Update();
	boss_barrier_->Update();

	// -------------------------------------------------
	// ↓ Debug
	// -------------------------------------------------
#ifdef _DEBUG

#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Boss::Draw() const {
	boss_body_->Draw();
	boss_core_->Draw();
	boss_leftHand_->Draw();
	boss_rightHand_->Draw();
	Engine::SetPipeline(PipelineType::NormalPipeline);
	boss_barrier_->Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　状態を遷移させる
//////////////////////////////////////////////////////////////////////////////////////////////////

void Boss::CheckBehaviorRequest() {
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();

		switch (behavior_) {
		case Behavior::ROOT:
			SetBehaviorState(std::make_unique<BossRootState>(this));
			break;
		case Behavior::ATTACK:
			SetBehaviorState(std::make_unique<BossAttackState>(this));
			CheckAttackType(attackType_);
			break;
		default:
			break;
		}

		// 振る舞いをリセット
		behaviorRequest_ = std::nullopt;
	}
}


void Boss::CheckMouseCursolCollision(const Matrix4x4& vpvpMat) {
	boss_rightHand_->CheckMouseCursorCollision(boss_rightHand_->GetTransform(), vpvpMat);
	boss_leftHand_->CheckMouseCursorCollision(boss_leftHand_->GetTransform(), vpvpMat);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　攻撃の種類を確認しそれに応じた初期化
//////////////////////////////////////////////////////////////////////////////////////////////////

void Boss::CheckAttackType(const AttackType& type) {
	switch (type) {
	case AttackType::GooSlap_Attack:
		// プレイヤーとの距離が近い方のみ攻撃
		if (near_ == "left") {
			boss_leftHand_->PrepareAttack(type);
			boss_rightHand_->SetIsAttackMove(false);
		} else {
			boss_rightHand_->PrepareAttack(type);
			boss_leftHand_->SetIsAttackMove(false);
		}
		break;

	case AttackType::ParSlap_Attack:
		// プレイヤーとの距離が近い方のみ攻撃
		if (near_ == "left") {
			boss_leftHand_->PrepareAttack(type);
			boss_rightHand_->SetIsAttackMove(false);
		} else {
			boss_rightHand_->PrepareAttack(type);
			boss_leftHand_->SetIsAttackMove(false);
		}
		break;
	case AttackType::Missile_Attack:
		boss_rightHand_->SetIsAttackMove(false);
		boss_leftHand_->SetIsAttackMove(false);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　ミサイル攻撃をする
//////////////////////////////////////////////////////////////////////////////////////////////////

void Boss::MissileAttack() {
	pGameScene_->AddMissile(playerPos_, boss_body_->GetTransform()->GetTranslation());
	Vector3 randomPos = playerPos_;
	randomPos.x += RandomFloat(-10.0f, 10.0f);
	pGameScene_->AddMissile(randomPos, boss_body_->GetTransform()->GetTranslation());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug処理
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void Boss::Debug_Gui() {
	if (ImGui::TreeNode("Boss")) {
		boss_body_->Debug_Gui();
		boss_core_->Debug_Gui();
		boss_leftHand_->Debug_Gui();
		boss_rightHand_->Debug_Gui();

		ImGui::Begin("Boss");
		std::string stateName = "nowState : " + state_->GetStateName();
		ImGui::Text(stateName.c_str());

		// eidterからファイル名を選ぶ
		if (ImGui::TreeNode("Left")) {
			leftHandEditer_->SelectAttack();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Right")) {
			rightHandEditer_->SelectAttack();
			ImGui::TreePop();
		}

		ImGui::InputText(".json##AttackName", &attackName_[0], sizeof(char) * 64);

		if (ImGui::Button("Save_Attack")) {
			Save();
		}

		if (ImGui::Button("Attack")) {
			behaviorRequest_ = Behavior::ATTACK;
		}

		ImGui::Text(near_.c_str());
		ImGui::Combo("attackType##type", &attackTypeNum_, "Goo\0Par\0Missile\0");
		attackType_ = static_cast<AttackType>(attackTypeNum_);

		ImGui::End();
		ImGui::TreePop();
	}
}

void Boss::Debug_Draw() {
	boss_core_->Debug_Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Save処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Boss::Save() {
	json root;
	// ファイル名前を保存する
	root[attackName_.c_str()]["left"] = leftHandEditer_->GetSelectFileName();
	root[attackName_.c_str()]["right"] = rightHandEditer_->GetSelectFileName();

	// 実際にファイルに保存する
	std::string filePath = attackDirectoryPath_ + attackName_.c_str() + ".json";

	std::ofstream ofs;
	// ファイルを書き込みように開く
	ofs.open(filePath);

	// ファイルオープンが出来ているか
	if (!ofs.fail()) {
		assert("not open File");
	}

	// ファイルにjson文字列を書き込む(インデント幅4)
	ofs << std::setw(4) << root << std::endl; // rootにあるデータをjson文字列に変換してファイルへ
	// 閉じる
	ofs.close();

}
#endif


void Boss::SetEditer(BossAttackEditer* left, BossAttackEditer* right) {
	leftHandEditer_ = left;
	rightHandEditer_ = right;
	boss_leftHand_->SetAttackEditer(left);
	boss_rightHand_->SetAttackEditer(right);

	boss_leftHand_->LoadAllFile();
	boss_rightHand_->LoadAllFile();

	if (boss_leftHand_->GetAnimetor() != nullptr) {
		leftHandEditer_->SetAnimations(boss_leftHand_->GetAnimetor()->GetAnimationNames());
	}
	if (boss_rightHand_->GetAnimetor() != nullptr) {
		rightHandEditer_->SetAnimations(boss_rightHand_->GetAnimetor()->GetAnimationNames());
	}
}