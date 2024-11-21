#include "PlayerRootState.h"
#include "Game/GameObject/Player/Player.h"

PlayerRootState::~PlayerRootState() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////
void PlayerRootState::Init() {
	stateName_ = "rootState";

	PlayerAnimator* animator = pPlayer_->GetPlayerAnimater();
	animator->NowToAfterTransition("defalut");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////
void PlayerRootState::Update() {

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void PlayerRootState::Debug_Gui() {

}
#endif // _DEBUG