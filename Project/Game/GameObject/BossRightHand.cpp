#include "BossRightHand.h"

BossRightHand::BossRightHand() {
}

BossRightHand::~BossRightHand() {
}

void BossRightHand::Finalize() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossRightHand::Init() {
	BaseGameObject::Init();
	SetObject("Right_Hand.gltf");
	SetAnimater("./Game/Resources/Model/Right_Hand/", "Right_Hand.gltf", true, true, true);

	SetMeshCollider("right_hand");

	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	adjust->AddItem(groupName_, "pos", transform_->GetTranslation());

	// 調整項目の適応
	AdaptAdjustment();

	moveIndex_ = 0;

	std::filesystem::path dire(attackDirectoryPath);
	if (!std::filesystem::exists(attackDirectoryPath)) {
		std::filesystem::create_directories(attackDirectoryPath);
	}

	std::filesystem::path direAnimation(animationDirectoryPath);
	if (!std::filesystem::exists(animationDirectoryPath)) {
		std::filesystem::create_directories(animationDirectoryPath);
	}

	transform_->SetQuaternion(Quaternion::AngleAxis(180.0f * toRadian, Vector3::UP()));
	transform_->SetScale(Vector3{ 0.5f, 0.5f, 0.5f });

	animationTime_ = 0.0f;
	animationTransitionTime_ = 0.0f;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossRightHand::Update() {
	if (isClicked_) {
		SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	} else {
		SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (isAttackMove_) {
		AttackMove(transform_.get());
	}

	if (animetor_ != nullptr) {
		animetor_->UpdateScript(animationTime_, animationTransitionTime_);
	}

	/*if (animetor_ != nullptr) {
		if (!animetor_->GetIsAnimationChange()) {
			animationTime_ += GameTimer::DeltaTime();
		}
		animetor_->UpdateScript(animationTime_, animationTransitionTime_);
		animationTime_ = std::fmod(animationTime_, animetor_->GetAnimationDuration());
	}*/

	BaseGameObject::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossRightHand::Draw() const {
	Engine::SetPipeline(PipelineType::NormalPipeline);
	BaseGameObject::Draw();

#ifdef _DEBUG
	Engine::SetPipeline(PipelineType::PrimitivePipeline);
	meshCollider_->Draw();
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　調整項目を適応させる
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossRightHand::AdaptAdjustment() {
	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	transform_->SetTranslaion(adjust->GetValue<Vector3>(groupName_, "pos"));
}

void BossRightHand::LoadAllFile() {
	pAttackEditer_->LoadAllFile(attackDirectoryPath);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void BossRightHand::Debug_Gui() {
	ImGui::Begin("Boss_RightHand");

	if (ImGui::CollapsingHeader("SetParameter")) {
		ImGui::BulletText("Set_Parameter");
		ImGui::Indent(20.0f);
		transform_->Debug_Gui();

		ShowEasingDebug(easeType_);	// easingを決める
		ImGui::DragFloat("moveTimeLimit", &moveTimeLimit_, 0.1f, 0.1f, 10.0f);	// 移動時間を決める

		// 決定をする
		if (isClicked_) {
			if (Input::IsTriggerKey(DIK_SPACE)) {
				pAttackEditer_->AddPoint(transform_->GetScale(), transform_->GetQuaternion(), transform_->GetTranslation(), easeType_, moveTimeLimit_);
			}
		}
		ImGui::Unindent(20.0f);
	}

	ImGui::Separator();
	if (ImGui::CollapsingHeader("Editer")) {
		ImGui::BulletText("Editer");
		ImGui::Indent(20.0f);
		pAttackEditer_->Debug_Gui(attackDirectoryPath);

		if (pAttackEditer_->GetHandMoves().size() != 0) {
			if (ImGui::Button("attackMove")) {
				isAttackMove_ = true;
				moveIndex_ = 0;
				moveTime_ = 0;
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

	// Parameter
	if (ImGui::CollapsingHeader("Debug_Parameter")) {
		ImGui::BulletText("Debug_Parameter");
		ImGui::Indent(20.0f);
		Debug_Axis();
		ImGui::DragFloat2("objectScreenPos", &objectScreenPos_.x, 1.0f);
		ImGui::Unindent(20.0f);
	}

	ImGui::Separator();
	if (ImGui::CollapsingHeader("Object")) {
		BaseGameObject::Debug_Gui();
	}

	ImGui::End();
}
#endif