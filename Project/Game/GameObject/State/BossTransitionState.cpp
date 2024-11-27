#include "BossTransitionState.h"
#include "Game/GameObject/Boss/Boss.h"
#include "Engine/Audio/AudioPlayer.h"

BossTransitionState::~BossTransitionState() {
}

void BossTransitionState::Init() {
	stateName_ = "TransitionState";
}

void BossTransitionState::Update() {
	if (pBoss_->GetIsTransitionForm()) {

	} else {
		pBoss_->GetBossBarrier()->SetEnableFunction(true);
		pBoss_->SetBehaviorRequest(Behavior::ROOT);
		pBoss_->SetBossForm(BossForm::SECOND);

		pBoss_->SetIsRecovery();

		pBoss_->GetBossBody()->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		pBoss_->GetBossLeftHand()->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		pBoss_->GetBossRightHand()->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));

		AudioPlayer::SinglShotPlay("barrierExpand.mp3", 0.3f);
	}
}

#ifdef _DEBUG
void BossTransitionState::Debug_Gui() {
	ImGui::Text(stateName_.c_str());
}
#endif // _DEBUG