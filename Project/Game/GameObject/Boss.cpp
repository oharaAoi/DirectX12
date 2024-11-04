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

	// -------------------------------------------------
	// ↓ handとbodyをペアレントさせる
	// -------------------------------------------------

	boss_leftHand_->GetTransform()->SetParent(boss_body_->GetTransform()->GetWorldMatrix());
	boss_rightHand_->GetTransform()->SetParent(boss_body_->GetTransform()->GetWorldMatrix());

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Boss::Update() {

	// -------------------------------------------------
	// ↓ 行列の更新
	// -------------------------------------------------
	boss_body_->Update();
	boss_leftHand_->Update();
	boss_rightHand_->Update();
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
		ImGui::TreePop();
	}
}
#endif