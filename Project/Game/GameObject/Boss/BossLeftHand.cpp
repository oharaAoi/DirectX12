#include "BossLeftHand.h"

BossLeftHand::BossLeftHand() {}
BossLeftHand::~BossLeftHand() {}

void BossLeftHand::Finalize() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossLeftHand::Init() {
	BaseGameObject::Init();
	SetObject("Left_Hand.gltf");
	SetAnimater("./Game/Resources/Model/Left_Hand/", "Left_Hand.gltf", true, true, true);

	SetMeshCollider("left_hand");
	BaseBossHand::Init();

	handType_ = HandType::LEFT_HAND;

	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	adjust->AddItem(groupName_, "pos", transform_->GetTranslation());

	// 調整項目の適応
	AdaptAdjustment();

	localRotate_ = Quaternion();
	initPos_ = transform_->GetTranslation();
	transform_->SetScale(Vector3(0.5f, 0.5f, 0.5f));

	moveIndex_ = 0;

	std::filesystem::path dire(attackDirectoryPath);
	if (!std::filesystem::exists(attackDirectoryPath)) {
		std::filesystem::create_directories(attackDirectoryPath);
	}

	std::filesystem::path direAnimation(animationDirectoryPath);
	if (!std::filesystem::exists(animationDirectoryPath)) {
		std::filesystem::create_directories(animationDirectoryPath);
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossLeftHand::Update() {
	BaseBossHand::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossLeftHand::Draw() const {
	BaseBossHand::Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　調整項目の適応
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossLeftHand::AdaptAdjustment() {
	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	transform_->SetTranslaion(adjust->GetValue<Vector3>(groupName_, "pos"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossLeftHand::LoadAllFile() {
	pAttackEditer_->LoadAllFile(attackDirectoryPath);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void BossLeftHand::Debug_Gui() {
	ImGui::Begin("Boss_LeftHand");

	bossHandHpUI_->Debug_Gui();

	if (ImGui::CollapsingHeader("SetParameter")) {
		ImGui::BulletText("Set_Parameter");
		ImGui::Indent(20.0f);
		
		BaseGameObject::Debug_Gui();

		ShowEasingDebug(easeType_);	// easingを決める
		ImGui::DragFloat("moveTimeLimit", &moveTimeLimit_, 0.1f);	// 移動時間を決める

		// 決定する
		if (isClicked_) {
			if (Input::IsTriggerKey(DIK_SPACE)) {
				pAttackEditer_->AddPoint(transform_->GetScale(), transform_->GetQuaternion(), transform_->GetTranslation(), easeType_, moveTimeLimit_);
			}
		}
		ImGui::Unindent(20.0f);
	}

	ImGui::Separator();
	if (ImGui::CollapsingHeader("Editer")) {
		ImGui::BulletText("Editer_Files");
		ImGui::Indent(20.0f);
		pAttackEditer_->Debug_Gui(attackDirectoryPath);

		if (pAttackEditer_->GetHandMoves().size() != 0) {
			if (ImGui::Button("attackMove")) {
				isAttackMove_ = true;
				moveIndex_ = 0;
				//moveTime_ = 0;
			}
		}

		// Animationの編集
		pAttackEditer_->Debug_Animation();
		if (ImGui::Button("test_play")) {
			animetor_->SetTransitionAnimation(pAttackEditer_->GetAnimationTransitionData().preAnimation, pAttackEditer_->GetAnimationTransitionData().afterAnimation);
			animationTransitionTime_ = pAttackEditer_->GetAnimationTransitionData().transitionTime;
		}

		pAttackEditer_->SaveLerpAnimation(animationDirectoryPath);

		ImGui::Unindent(20.0f);
	}

	ImGui::Separator();
	if (ImGui::CollapsingHeader("Debug_Parameter")) {
		ImGui::BulletText("Debug_Parameter");
		ImGui::Indent(20.0f);
		Debug_Axis();
		ImGui::DragFloat2("objectScreenPos", &objectScreenPos_.x, 1.0f);
		ImGui::Unindent(20.0f);
	}

	ImGui::End();
}
#endif