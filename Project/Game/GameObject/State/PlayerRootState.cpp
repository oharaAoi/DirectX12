#include "PlayerRootState.h"

PlayerRootState::~PlayerRootState() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////
void PlayerRootState::Init() {
	stateName_ = "rootState";
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