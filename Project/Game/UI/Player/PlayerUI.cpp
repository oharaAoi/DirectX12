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
	sprites_["clutch"] = Engine::CreateSprite("star.png");
	sprites_["clutch"]->SetScale(Vector2{0.4f, 0.4f});
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerUI::Update() {
	sprites_["clutch"]->SetTranslate(playerScreenPos_);
	sprites_["clutch"]->Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerUI::Draw(bool canAttackBoss) {
	if (canAttackBoss) {
		sprites_["clutch"]->Draw();
	}
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