#include "PlayerJumpState.h"
#include "Game/GameObject/Player/Player.h"
#include "Engine/Utilities/AdjustmentItem.h"

PlayerJumpState::~PlayerJumpState() {
}

void PlayerJumpState::Init() {
	stateName_ = "playerJumpState";
	// 基本の情報の初期化
	information_.FromJson(AdjustmentItem::GetData(stateName_, stateName_));
	// 特有の行動の初期化
	velocity_ = Vector3::ZERO();
	acceleration_ = Vector3::ZERO();;
	work_.jumpHight = 2.0f;
	work_.FromJson(AdjustmentItem::GetData(stateName_, "work"));

	velocity_.y = 6.0f;
	acceleration_.y = -9.8f;
	pPlayer_->GetAnimetor()->TransitionAnimation(information_.animationName, 0.5f);
	pPlayer_->GetAnimetor()->GetAnimationClip()->SetIsLoop(false);
}

void PlayerJumpState::Update() {
	Jump();

	JumpAttack();
}


void PlayerJumpState::Jump() {
	// 移動する
	Vector3 translate = pPlayer_->GetTransform()->GetTranslation();
	velocity_ += acceleration_ * GameTimer::DeltaTime();
	translate += velocity_ * GameTimer::DeltaTime();

	// 限界の範囲を指定
	if (translate.y <= 0.0f) {
		translate.y = 0.0f;
		pPlayer_->SetIsJump(false);
		pPlayer_->SetBehaviorRequest(Behavior::DEFAULT);
		pPlayer_->GetAnimetor()->GetAnimationClip()->ResetAnimation("idle");
		pPlayer_->GetAnimetor()->GetAnimationClip()->SetIsLoop(true);

	}
	// 実際に代入する
	pPlayer_->GetTransform()->SetTranslaion((translate));
}

void PlayerJumpState::JumpAttack() {
	if (Input::GetIsPadTrigger(BUTTON_X)) {
		pPlayer_->SetBehaviorRequest(Behavior::ATTACK);
		pPlayer_->SetAttackStep(AttackStep::Step_JUMPATTACK);
		pPlayer_->SetIsAttack(true);
	}
}

#ifdef _DEBUG
void PlayerJumpState::Debug_Gui() {
	ImGui::Text(stateName_.c_str());

	if (ImGui::TreeNode("Information")) {
		information_.animationName = pPlayer_->GetAnimetor()->SelectAnimationName();
		ImGui::Text(information_.animationName.c_str());
		if (ImGui::Button("Save")) {
			AdjustmentItem::Save(stateName_, information_.ToJson(stateName_));
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("work")) {
		ImGui::DragFloat("jumpHight", &work_.jumpHight, 0.1f);
	
		if (ImGui::Button("Save")) {
			AdjustmentItem::Save(stateName_, work_.ToJson("work"));
		}
		ImGui::TreePop();
	}
}
#endif // _DEBUG