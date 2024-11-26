#include "BossRootState.h"
#include "Game/GameObject/Boss/Boss.h"

BossRootState::~BossRootState() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossRootState::Init() {
	stateName_ = "rootState";

	pBoss_->GetBossRightHand()->ChangeRootMove(pBoss_->GetBossRightHand());
	pBoss_->GetBossRightHand()->ChangeRootMove(pBoss_->GetBossLeftHand());

	work_.waitTime = 4.0f;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void BossRootState::Update() {
	pBoss_->GetBossRightHand()->AnimationTimeIncrement(true);
	pBoss_->GetBossLeftHand()->AnimationTimeIncrement(true);

#ifdef _DEBUG
	if (isStop_) {
		return;
	}
#endif // _DEBUG

	// 待機時間を減らす
	work_.waitTime -= GameTimer::DeltaTime();
	
	if (work_.waitTime < 0.0f) {
		pBoss_->SetBehaviorRequest(Behavior::ATTACK);
		// 形態によって上限を決める
		if (pBoss_->GetBossForm() == BossForm::FIRST) {
			AttackType type = static_cast<AttackType>(RandomInt(0,(static_cast<int>(AttackType::Missile_Attack))));
			while (type == pBoss_->GetAttackType()) {
				type = static_cast<AttackType>(RandomInt(0, (static_cast<int>(AttackType::Missile_Attack))));
			}
			pBoss_->SetAttackType(type);

		} else {
			AttackType type = static_cast<AttackType>(RandomInt(0, (static_cast<int>(AttackType::TOTAL) - 1)));
			while (type == pBoss_->GetAttackType()) {
				type = static_cast<AttackType>(RandomInt(0, (static_cast<int>(AttackType::TOTAL) - 1)));
			}
			pBoss_->SetAttackType(type);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void BossRootState::Debug_Gui() {
	ImGui::Text(stateName_.c_str());
	ImGui::SliderFloat("waitTime", &work_.waitTime, 0.0f, 10.0f);
	ImGui::Checkbox("stop", &isStop_);
}
#endif // _DEBUG