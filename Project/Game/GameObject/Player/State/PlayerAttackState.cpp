#include "PlayerAttackState.h"
#include "Game/GameObject/Player/Player.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/System/Audio/AudioPlayer.h"

PlayerAttackState::~PlayerAttackState() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerAttackState::Init() {
	stateName_ = "playerAttackState";

	colliderActiveTimes_.FromJson(AdjustmentItem::GetData("AttacCollider", stateName_));

	work_.time = 0.0f;
	work_.timeLimit = 1.0f;

	colliderActiveTime_ = 0.0f;

	isComb_ = false;

	velocity_ = { 0.0f, 4.0f, 0.0f };
	acceleration_.y = -9.8f;

	information_.FromJson(AdjustmentItem::GetData(stateName_, stateName_));

	pPlayer_->GetAnimetor()->GetAnimationClip()->SetIsLoop(false);
	if (pPlayer_->GetAttackStep() != AttackStep::Step_JUMPATTACK) {
		pPlayer_->GetAnimetor()->TransitionAnimation(information_.animationName, 0.1f);
	} else {
		pPlayer_->GetAnimetor()->TransitionAnimation("fallAttack", 0.1f);
		pPlayer_->GetAnimetor()->SetAnimationSpeed(1.3f);
	}

	combNum_ = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerAttackState::Update() {
	//if (pPlayer_->GetIsStateDebug()) {
	//	return ;
	//}

	colliderActiveTime_ += GameTimer::DeltaTime();

	if (pPlayer_->GetAttackStep() != AttackStep::Step_JUMPATTACK) {
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

	if (!pPlayer_->GetAnimetor()->GetAnimationClip()->GetIsChange()) {
		if (pPlayer_->GetAnimetor()->GetAnimationClip()->GetIsAnimationFinish()) {
			if (!isComb_) {
				pPlayer_->SetBehaviorRequest(Behavior::DEFAULT);
				pPlayer_->GetAnimetor()->TransitionAnimation("idle", 0.1f);
				pPlayer_->GetAnimetor()->GetAnimationClip()->SetIsLoop(true);
				pPlayer_->SetIsAttack(false);
			}
		}
	}

	if (isComb_) {
		if (!pPlayer_->GetAnimetor()->GetAnimationClip()->GetIsReservation()) {
			isComb_ = false;
			combNum_++;
			if (combNum_ == 1) {
				AudioPlayer::SinglShotPlay("attack2.mp3", 0.6f);
				pPlayer_->ResetTrail();
				colliderActiveTime_ = 0.0f;
			} else if (combNum_ == 2) {
				AudioPlayer::SinglShotPlay("attack3.mp3", 0.6f);
				pPlayer_->ResetTrail();
				colliderActiveTime_ = 0.0f;
			} else if (combNum_ == 3) {
				AudioPlayer::SinglShotPlay("attack4.mp3", 0.6f);
				pPlayer_->ResetTrail();
				colliderActiveTime_ = 0.0f;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　次の攻撃の確認
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerAttackState::CheckNextAttack() {
	if (!isComb_) {
		if (!pPlayer_->GetAnimetor()->GetAnimationClip()->GetIsChange()) {
			if (Input::GetIsPadTrigger(BUTTON_X)) {
				if (pPlayer_->GetAttackStep() < AttackStep::Step_FOUR) {
					pPlayer_->CombStepUp();
					Attack(pPlayer_->GetAttackStep());
					isComb_ = true;
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓ 攻撃の実行
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerAttackState::Attack(AttackStep step) {
	const auto& it = functionMap_.find(step);
	if (it != functionMap_.end()) {
		(this->*(it->second))(); // メンバー関数ポインタを使って関数を呼び出す
	}
}

void PlayerAttackState::FirstAttack() {
	pPlayer_->GetAnimetor()->TransitionAnimation(information_.animationName, 0.1f);
}

void PlayerAttackState::SecondAttack() {
	pPlayer_->GetAnimetor()->GetAnimationClip()->ReservationAnimation(information_.animationName, "attack2", 0.1f, 0.85f);
}

void PlayerAttackState::TheradAttack() {
	pPlayer_->GetAnimetor()->GetAnimationClip()->ReservationAnimation("attack2", "attack3", 0.1f, 0.9f);
}

void PlayerAttackState::FourAttack() {
	pPlayer_->GetAnimetor()->GetAnimationClip()->ReservationAnimation("attack3", "attack4", 0.1f, 0.9f);
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
		pPlayer_->SetIsJump(false);
		pPlayer_->SetBehaviorRequest(Behavior::DEFAULT);
		pPlayer_->GetAnimetor()->TransitionAnimation("idle", 0.1f);
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

	ImGui::BulletText("colliderActiveTime");
	ImGui::DragFloat("firstTime", &colliderActiveTimes_.activeStartTime[0], 0.01f);
	ImGui::DragFloat("secondTime", &colliderActiveTimes_.activeStartTime[1], 0.01f);
	ImGui::DragFloat("theradTime", &colliderActiveTimes_.activeStartTime[2], 0.01f);
	ImGui::DragFloat("fourTime", &colliderActiveTimes_.activeStartTime[3], 0.01f);

	information_.animationName = pPlayer_->GetAnimetor()->SelectAnimationName();
	ImGui::Text(information_.animationName.c_str());
	if (ImGui::Button("Save")) {
		AdjustmentItem::Save(stateName_, information_.ToJson(stateName_));
		AdjustmentItem::Save("AttacCollider", colliderActiveTimes_.ToJson(stateName_));
	}

	if (ImGui::Button("Reset")) {
		this->Init();
	}
}
#endif // _DEBUG