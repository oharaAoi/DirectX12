#include "PlayerBeAttackedState.h"
#include "Game/GameObject/Player/Player.h"

PlayerBeAttackedState::~PlayerBeAttackedState() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerBeAttackedState::Init() {
	stateName_ = "BeAttacked";

	knockBackSpeed_ = 14.0f;

	// 左に飛ぶ
	if (pPlayer_->GetTargetRotate() <= 0.0f) {
		knockBackVelocity_ = { -1.0f, 0.0f, 0.0f };
	} else {
		knockBackVelocity_ = { 1.0f, 0.0f, 0.0f };
	}

	switch (pPlayer_->GetBeAttackedType()) {
	case BeAttackedType::NORMAL_HITED:
		knockBackFunction_ = std::bind(&PlayerBeAttackedState::NormalKnockBack, this);
		work_.knockBackTime = 0.0f;
		work_.knockBackTimeLimit = 0.3f;
		break;

	case BeAttackedType::SLAP_ATTACKED:
		knockBackFunction_ = std::bind(&PlayerBeAttackedState::SlapedKnockBack, this);
		work_.knockBackTime = 0.0f;
		work_.knockBackTimeLimit = 1.0f;
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerBeAttackedState::Update() {
	Reaction();
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　リアクションをとる
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerBeAttackedState::TimeUpdate() {
	// 時間の更新
	work_.knockBackTime += GameTimer::DeltaTime();
	if (work_.knockBackTime >= work_.knockBackTimeLimit) {
		pPlayer_->SetBehaviorRequest(PlayerState::Default);
	}
}

void PlayerBeAttackedState::Reaction() {
	if (knockBackFunction_) {
		knockBackFunction_();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　普通にぶつかった時の処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerBeAttackedState::NormalKnockBack() {
	// 座標の移動
	Vector3 pos = pPlayer_->GetTransform()->GetTranslation();
	pos += (knockBackVelocity_ * knockBackSpeed_) * GameTimer::DeltaTime();
	pPlayer_->GetTransform()->SetTranslaion(pos);

	// speedを時間で変更する
	knockBackSpeed_ = std::lerp(14.0f, 3.0f, Ease::Out::Expo(work_.knockBackTime / work_.knockBackTimeLimit));

	TimeUpdate();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　叩き潰された時
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerBeAttackedState::SlapedKnockBack() {
	
	Vector3 scale = pPlayer_->GetTransform()->GetScale();
	float halfTime = (work_.knockBackTimeLimit / 2.0f);
	if (work_.knockBackTime < halfTime) {
		scale.y = std::lerp(1.0f, 0.2f, Ease::Out::Expo(work_.knockBackTime / halfTime));
	} else {
		scale.y = std::lerp(0.2f, 1.0f, Ease::Out::Elastic((work_.knockBackTime - halfTime) / halfTime));
	}
	pPlayer_->GetTransform()->SetScale(scale);

	TimeUpdate();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void PlayerBeAttackedState::Debug_Gui() {
}
#endif // _DEBUG