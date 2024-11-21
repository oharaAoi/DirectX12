#include "PlayerBeAttackedState.h"
#include "Game/GameObject/Player.h"

PlayerBeAttackedState::~PlayerBeAttackedState() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerBeAttackedState::Init() {
	stateName_ = "BeAttacked";

	work_.knockBackTime = 0.3f;
	knockBackSpeed_ = 14.0f;

	// 左に飛ぶ
	if (pPlayer_->GetTargetRotate() <= 0.0f) {
		knockBackVelocity_ = { -1.0f, 0.0f, 0.0f };
	} else {
		knockBackVelocity_ = { 1.0f, 0.0f, 0.0f };
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerBeAttackedState::Update() {
	// 座標の移動
	Vector3 pos = pPlayer_->GetTransform()->GetTranslation();
	pos += (knockBackVelocity_ * knockBackSpeed_) * GameTimer::DeltaTime();
	pPlayer_->GetTransform()->SetTranslaion(pos);

	// speedを時間で変更する
	knockBackSpeed_ = std::lerp(3.0f, 14.0f, Ease::Out::Expo(work_.knockBackTime / 0.3f));

	// 時間の更新
	work_.knockBackTime -= GameTimer::DeltaTime();

	if (work_.knockBackTime <= 0.0f) {
		pPlayer_->SetBehaviorRequest(PlayerState::Default);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void PlayerBeAttackedState::Debug_Gui() {
}
#endif // _DEBUG