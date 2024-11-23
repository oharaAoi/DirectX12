#include "Boss.h"
#include "Game/Scene/GameScene.h"
#include "Game/GameObject/State/BossRootState.h"
#include "Game/GameObject/State/BossAttackState.h"
#include "Game/GameObject/State/BossTransitionState.h"

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
	body_ = std::make_unique<BossBody>();
	body_->Init();

	core_ = std::make_unique<BossCore>();
	core_->Init();

	leftHand_ = std::make_unique<BossLeftHand>();
	leftHand_->Init();

	rightHand_ = std::make_unique<BossRightHand>();
	rightHand_->Init();

	barrier_ = std::make_unique<BossBarrier>();
	barrier_->Init();

	state_ = std::make_unique<BossRootState>(this);

	// -------------------------------------------------
	// ↓ handとbodyをペアレントさせる
	// -------------------------------------------------

	core_->GetTransform()->SetParent(body_->GetTransform()->GetWorldMatrix());
	leftHand_->GetTransform()->SetParent(body_->GetTransform()->GetWorldMatrix());
	rightHand_->GetTransform()->SetParent(body_->GetTransform()->GetWorldMatrix());
	barrier_->GetTransform()->SetParent(body_->GetTransform()->GetWorldMatrix());

	// -------------------------------------------------
	// ↓ 攻撃の情報が入ったフォルダを作成
	// -------------------------------------------------

	std::filesystem::path dire(attackDirectoryPath_);
	if (!std::filesystem::exists(attackDirectoryPath_)) {
		std::filesystem::create_directories(attackDirectoryPath_);
	}

	behaviorRequest_ = Behavior::ROOT;
	CheckBehaviorRequest();

	form_ = BossForm::FIRST;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Boss::Update() {
	CheckBehaviorRequest();
	state_->Update();

	bossHp_ = core_->GetHp();
	if (bossHp_ <= 0.0f) {
		if (form_ == BossForm::FIRST) {
			if (behavior_ != Behavior::ATTACK) {
				behaviorRequest_ = Behavior::TRANSITION;
				isTransitionForm_ = true;
			}
		}
	}

	// -------------------------------------------------
	// ↓ playerの座標から右手と左手でどちらが近いかを計算しておく
	// -------------------------------------------------

	float playerToLeft = (playerPos_ - leftHand_->GetTransform()->GetTranslation()).Length();
	float playerToRight = (playerPos_ - rightHand_->GetTransform()->GetTranslation()).Length();

	if (playerToLeft < playerToRight) {
		leftHand_->SetIsNear(true);
		rightHand_->SetIsNear(false);
		near_ = "left";
	} else {
		leftHand_->SetIsNear(false);
		rightHand_->SetIsNear(true);
		near_ = "right";
	}

	// -------------------------------------------------
	// ↓ 行列の更新
	// -------------------------------------------------

	leftHand_->SetBodyPos(body_->GetTransform()->GetTranslation());
	rightHand_->SetBodyPos(body_->GetTransform()->GetTranslation());

	body_->Update();
	core_->Update();
	leftHand_->Update();
	rightHand_->Update();
	barrier_->Update();

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
	body_->Draw();
	core_->Draw();
	leftHand_->Draw();
	rightHand_->Draw();
}

void Boss::PostDraw() const {
	Engine::SetPipeline(PipelineType::NormalPipeline);
	barrier_->Draw();
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
		case Behavior::TRANSITION:
			SetBehaviorState(std::make_unique<BossTransitionState>(this));
			break;
		default:
			break;
		}

		// 振る舞いをリセット
		behaviorRequest_ = std::nullopt;
	}
}


void Boss::CheckMouseCursolCollision(const Matrix4x4& vpvpMat) {
	rightHand_->CheckMouseCursorCollision(rightHand_->GetTransform(), vpvpMat);
	leftHand_->CheckMouseCursorCollision(leftHand_->GetTransform(), vpvpMat);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　攻撃の種類を確認しそれに応じた初期化
//////////////////////////////////////////////////////////////////////////////////////////////////

void Boss::CheckAttackType(const AttackType& type) {
	switch (type) {
	case AttackType::GooSlap_Attack:
		// プレイヤーとの距離が近い方のみ攻撃
		if (near_ == "left") {
			leftHand_->PrepareAttack(type);
			rightHand_->SetIsAttackMove(false);

			leftHand_->GetAnimetor()->SetTransitionAnimation("stand_by", "slam");
		} else {
			rightHand_->PrepareAttack(type);
			leftHand_->SetIsAttackMove(false);

			rightHand_->GetAnimetor()->SetTransitionAnimation("stand_by", "slam");
		}
		break;

	case AttackType::ParSlap_Attack:
		// プレイヤーとの距離が近い方のみ攻撃
		if (near_ == "left") {
			leftHand_->PrepareAttack(type);
			rightHand_->SetIsAttackMove(false);
		} else {
			rightHand_->PrepareAttack(type);
			leftHand_->SetIsAttackMove(false);
		}
		break;
	case AttackType::Missile_Attack:
		rightHand_->SetIsAttackMove(false);
		leftHand_->SetIsAttackMove(false);
		break;

	case AttackType::MowDown_Attack:
		// プレイヤーとの距離が近い方のみ攻撃
		if (near_ == "left") {
			leftHand_->PrepareAttack(type);
			rightHand_->SetIsAttackMove(false);

		} else {
			rightHand_->PrepareAttack(type);
			leftHand_->SetIsAttackMove(false);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　ミサイル攻撃をする
//////////////////////////////////////////////////////////////////////////////////////////////////

void Boss::MissileAttack() {
	pGameScene_->AddMissile(playerPos_, body_->GetTransform()->GetTranslation());
	Vector3 randomPos = playerPos_;
	randomPos.x += RandomFloat(-10.0f, 10.0f);
	pGameScene_->AddMissile(randomPos, body_->GetTransform()->GetTranslation());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug処理
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void Boss::Debug_Gui() {
	if (ImGui::TreeNode("Boss")) {
		body_->Debug_Gui();
		core_->Debug_Gui();
		leftHand_->Debug_Gui();
		rightHand_->Debug_Gui();
		if (barrier_ != nullptr) {
			barrier_->Debug_Gui();
		}

		ImGui::Begin("Boss");

		// 状態
		ImGui::BulletText("State");
		state_->Debug_Gui();

		// 攻撃
		ImGui::BulletText("Attack");
		ImGui::Text(near_.c_str());
		ImGui::Combo("attackType##type", &attackTypeNum_, "Goo\0Par\0Missile\0MowDown\0");
		attackType_ = static_cast<AttackType>(attackTypeNum_);
		if (ImGui::Button("Set")) {
			isSetAttack_ = true;
			leftHand_->PrepareAttack(attackType_);
		}
		ImGui::SameLine();
		if (ImGui::Button("Attack")) {
			behaviorRequest_ = Behavior::ATTACK;
			isSetAttack_ = false;
		}

		if (isSetAttack_) {
			leftHand_->GetIAttack()->Debug_Gui();
		}

		// eidterからファイル名を選ぶ
		ImGui::BulletText("HandAttackSave");
		if (ImGui::TreeNode("Left")) {
			
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

		ImGui::End();
		ImGui::TreePop();
	}
}

void Boss::Debug_Draw() {
	core_->Debug_Draw();
	barrier_->Debug_Draw();
	rightHand_->Debug_Draw();
	leftHand_->Debug_Draw();
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
	leftHand_->SetAttackEditer(left);
	rightHand_->SetAttackEditer(right);

	leftHand_->LoadAllFile();
	rightHand_->LoadAllFile();

	if (leftHand_->GetAnimetor() != nullptr) {
		leftHandEditer_->SetAnimations(leftHand_->GetAnimetor()->GetAnimationNames());
	}
	if (rightHand_->GetAnimetor() != nullptr) {
		rightHandEditer_->SetAnimations(rightHand_->GetAnimetor()->GetAnimationNames());
	}
}