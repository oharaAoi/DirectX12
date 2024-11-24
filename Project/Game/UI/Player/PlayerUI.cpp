#include "PlayerUI.h"
#include "Engine/Engine.h"


PlayerUI::PlayerUI() {
}

PlayerUI::~PlayerUI() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerUI::Init() {
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerUI::Update() {
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerUI::Draw(bool canAttackBoss) {
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Setter
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerUI::SetPlayerScreenPos(const Matrix4x4& playerMat, const Matrix4x4& vpvpMat) {
	Vector3 pos = Transform(Vector3::ZERO(), playerMat * vpvpMat);
	playerScreenPos_ = { pos.x, pos.y };
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
void PlayerUI::Debug_Gui() {

}
#endif