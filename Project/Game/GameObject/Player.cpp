#include "Player.h"
#include "Game/Scene/GameScene.h"


Player::Player() {}
Player::~Player() {}

void Player::Finalize() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Init() {
	BaseGameObject::Init();
	SetObject("cube.obj");
	transform_->SetScale({ 1.0f, 1.0f, 1.0f });
	transform_->SetTranslaion({ 0.0f, 0.0f, 0.0f });
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Update() {

	Move();
	Clutch();

	BaseGameObject::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Draw() const {
	BaseGameObject::Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void Player::Debug_Gui() {
	if (ImGui::TreeNode("player")) {
		ImGui::Begin("player");
		if (ImGui::TreeNode("Move")) {

			ImGui::DragFloat("MoveSpeed", &moveSpeed_, 0.01f, 0.0f, 20.0f);
			Vector3 pos = transform_->GetTranslation();
			ImGui::DragFloat3("Trans", &pos.x);

			ImGui::TreePop();
		}


		ImGui::End();
		ImGui::TreePop();
	}
}
#endif

void Player::Move() {
	Vector3 pos = transform_->GetTranslation();
	if (Input::IsPressKey(DIK_A)) {
		pos.x -= moveSpeed_ * GameTimer::DeltaTime();
	}
	if (Input::IsPressKey(DIK_D)) {
		pos.x += moveSpeed_ * GameTimer::DeltaTime();
	}

	transform_->SetTranslaion(pos);
}

void Player::Clutch() {

	if (!isReturnClutch_) {// 最大まで伸びて、戻る状態じゃない時
		if (Input::IsPressMouse(0)) {
			isStretchClutch_ = true;
		}
	}


	if (isStretchClutch_) {
		
	}



}
