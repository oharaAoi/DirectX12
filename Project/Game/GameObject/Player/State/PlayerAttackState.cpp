#include "PlayerAttackState.h"
#include "Game/GameObject/Player/Player.h"
#include "Engine/Utilities/AdjustmentItem.h"

PlayerAttackState::~PlayerAttackState() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerAttackState::Init() {
	stateName_ = "playerAttackState";

	work_.time = 0.0f;
	work_.timeLimit = 1.0f;

	isComb_ = false;

	velocity_ = { 0.0f, 4.0f, 0.0f };
	acceleration_.y = -9.8f;

	information_.FromJson(AdjustmentItem::GetData(stateName_, stateName_));

	pPlayer_->GetAnimetor()->GetAnimationClip()->SetIsLoop(false);
	if (pPlayer_->GetAttackStep() != AttackStep::JUMPATTACK) {
		pPlayer_->GetAnimetor()->TransitionAnimation(information_.animationName);
	} else {
		pPlayer_->GetAnimetor()->TransitionAnimation("fallAttack");
		pPlayer_->GetAnimetor()->SetAnimationSpeed(1.3f);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerAttackState::Update() {
	if (pPlayer_->GetIsStateDebug()) {
		return ;
	}

	if (pPlayer_->GetAttackStep() != AttackStep::JUMPATTACK) {
		CombAttack();
	} else {
		JumpAttack();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　攻撃の内容
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerAttackState::CombAttack() {
	CheckNextAttack();

	if (pPlayer_->GetAnimetor()->GetAnimationClip()->GetIsAnimationFinish()) {
		if (!isComb_) {
			pPlayer_->SetBehaviorRequest(Behavior::DEFAULT);
			pPlayer_->GetAnimetor()->GetAnimationClip()->SetIsLoop(true);
			pPlayer_->SetIsAttack(false);
		} else {
			//pPlayer_->GetAnimetor()->TransitionAnimation("attack2");
			//pPlayer_->GetAnimetor()->GetAnimationClip()->SetLerpAnimation(information_.animationName, "attack2");
			pPlayer_->SetAttackStep(AttackStep::SECOND);
			isComb_ = false;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　次の攻撃の確認
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerAttackState::CheckNextAttack() {
	if (!isComb_) {
		if (Input::GetIsPadTrigger(BUTTON_X)) {
			pPlayer_->GetAnimetor()->GetAnimationClip()->SetAnimationReservation(information_.animationName, "attack2", 0.85f);
			isComb_ = true;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　ジャンプ時の攻撃
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerAttackState::JumpAttack() {
	// 移動する
	Vector3 translate = pPlayer_->GetTransform()->GetTranslation();
	velocity_ += acceleration_ * GameTimer::DeltaTime();
	translate += velocity_ * GameTimer::DeltaTime();

	// 限界の範囲を指定
	if (translate.y <= 0.0f) {
		translate.y = 0.0f;
		pPlayer_->SetIsAttack(false);
		pPlayer_->SetBehaviorRequest(Behavior::DEFAULT);
		pPlayer_->GetAnimetor()->TransitionAnimation("idle");
		pPlayer_->GetAnimetor()->GetAnimationClip()->SetIsLoop(true);
		pPlayer_->GetAnimetor()->SetAnimationSpeed(1.0f);
	}
	// 実際に代入する
	pPlayer_->GetTransform()->SetTranslaion((translate));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void PlayerAttackState::Debug_Gui() {
	ImGui::Text(stateName_.c_str());
	ImGui::SliderFloat("work_.time", &work_.time, 0.0f, work_.timeLimit);

	information_.animationName = pPlayer_->GetAnimetor()->SelectAnimationName();
	ImGui::Text(information_.animationName.c_str());
	if (ImGui::Button("Save")) {
		AdjustmentItem::Save(stateName_, information_.ToJson(stateName_));
	}
}
#endif // _DEBUG