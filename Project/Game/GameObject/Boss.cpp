#include "Boss.h"

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

	boss_leftHand_ = std::make_unique<BossLeftHand>();
	boss_leftHand_->Init();

	boss_rightHand_ = std::make_unique<BossRightHand>();
	boss_rightHand_->Init();

	state_ = std::make_unique<BossRootState>(this);

	// -------------------------------------------------
	// ↓ handとbodyをペアレントさせる
	// -------------------------------------------------

	boss_leftHand_->GetTransform()->SetParent(boss_body_->GetTransform()->GetWorldMatrix());
	boss_rightHand_->GetTransform()->SetParent(boss_body_->GetTransform()->GetWorldMatrix());

	// -------------------------------------------------
	// ↓ 攻撃の情報が入ったフォルダを作成
	// -------------------------------------------------

	std::filesystem::path dire(attackDirectoryPath_);
	if (!std::filesystem::exists(attackDirectoryPath_)) {
		std::filesystem::create_directories(attackDirectoryPath_);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Boss::Update() {
	state_->Update();

	// -------------------------------------------------
	// ↓ 行列の更新
	// -------------------------------------------------
	boss_body_->Update();
	boss_leftHand_->Update();
	boss_rightHand_->Update();

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
	boss_leftHand_->Draw();
	boss_rightHand_->Draw();
}


void Boss::CheckMouseCursolCollision(const Matrix4x4& vpvpMat) {
	boss_rightHand_->CheckMouseCursorCollision(vpvpMat);
	boss_leftHand_->CheckMouseCursorCollision(vpvpMat);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug処理
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void Boss::Debug_Gui() {
	if (ImGui::TreeNode("Boss")) {
		boss_body_->Debug_Gui();
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

		ImGui::End();
		ImGui::TreePop();
	}
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