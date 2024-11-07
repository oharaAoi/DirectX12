#include "Player.h"
#include "Game/Scene/GameScene.h"

uint32_t Player::score_ = 0;

Player::Player() {
}

Player::~Player() {
}

void Player::Finalize() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Init() {
	BaseGameObject::Init();
	SetObject("skin.obj");
	transform_->SetScale({ 0.5f, 0.5f, 0.5f });
	transform_->SetTranslaion({ 0.0f, -0.5f, 0.0f });

	score_ = 0;

	shotEnergy_ = 1.0f;
	shotEnergyRaito_ = 1.0f;

	test_ = std::make_unique<BaseGameObject>();
	test_->Init();
	test_->SetObject("cube.obj");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Update() {
	if (Input::IsPressKey(DIK_SPACE)) {
		Shot();
	} else {
		isShot_ = false;
		shotEnergy_ += GameTimer::DeltaTime();
		shotEnergyRaito_ = shotEnergy_ / 1.0f;
	}

	if (Input::GetIsPadTrigger(R_SHOULDER)) {
		Shot();
	}

	shotEnergy_ = std::clamp(shotEnergy_, 0.0f, 1.0f);

	test_->GetTransform()->SetTranslaion(reticlrPos_);

	test_->Update();
	BaseGameObject::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Draw() const {
	BaseGameObject::Draw();
	test_->Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　攻撃関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Shot() {
	isShot_ = true;
	Vector3 dire = (reticlrPos_ - worldPos_).Normalize();
	shotQuaternion_ = Quaternion::LookAt(worldPos_, reticlrPos_);
	shotEnergy_ -= GameTimer::DeltaTime();
	shotEnergyRaito_ = shotEnergy_ / 1.0f;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void Player::Debug_Gui() {
	if (ImGui::TreeNode("player")) {
		ImGui::Begin("player");

		ImGui::Text("score : %d", score_);
		ImGui::Text("shotEnergyRaito : %f", shotEnergyRaito_);

		ImGui::End();
		ImGui::TreePop();
	}
}
#endif
