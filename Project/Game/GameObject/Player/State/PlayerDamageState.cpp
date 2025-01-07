#include "PlayerDamageState.h"
#include "Game/GameObject/Player/Player.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/System/Audio/AudioPlayer.h"

PlayerDamageState::~PlayerDamageState() {
}

void PlayerDamageState::Init() {
	stateName_ = "playerDamageState";

	work_.time = 0.0f;
	work_.timeLimit = 1.0f;
	work_.strength = 20.0f;

	information_.FromJson(AdjustmentItem::GetData(stateName_, stateName_));
	pPlayer_->GetAnimetor()->TransitionAnimation(information_.animationName, 0.2f);

	AudioPlayer::SinglShotPlay("damege.mp3", 0.6f);
}

void PlayerDamageState::Update() {
	if (work_.time >= work_.timeLimit) {
		pPlayer_->SetBehaviorRequest(Behavior::DEFAULT);
		return;
	}

	work_.time += GameTimer::DeltaTime();
	KnockBack();
}

void PlayerDamageState::KnockBack() {
	Vector3 pos = pPlayer_->GetTransform()->GetTranslation();
	pos += (pPlayer_->GetKnockBackVelocity() * work_.strength) * GameTimer::DeltaTime();
	pPlayer_->GetTransform()->SetTranslaion(pos);

	work_.strength *= 0.8f;
}

#ifdef _DEBUG
void PlayerDamageState::Debug_Gui() {
	ImGui::Text(stateName_.c_str());
	information_.animationName = pPlayer_->GetAnimetor()->SelectAnimationName();
	ImGui::Text(information_.animationName.c_str());
	if (ImGui::Button("Save")) {
		AdjustmentItem::Save(stateName_, information_.ToJson(stateName_));
	}
}
#endif // _DEBUG