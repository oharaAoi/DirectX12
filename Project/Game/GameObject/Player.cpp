#include "Player.h"
#include "Game/Scene/GameScene.h"

uint32_t Player::score_ = 0;

Player::Player() {
}

Player::~Player() {
	Finalize();
}

void Player::Finalize() {
	shotSe_->Finalize();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Init() {
	BaseGameObject::Init();
	SetObject("cart.obj");
	transform_->SetScale({ 0.5f, 0.5f, 0.5f });
	transform_->SetTranslaion({ 0.0f, -0.4f, -0.2f });

	score_ = 0;

	shotEnergy_ = 1.0f;
	shotEnergyRaito_ = 1.0f;

	shotSe_ = std::make_unique<AudioPlayer>();
	shotSe_->Init("shot.mp3");
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
		shotSe_->Stop();
	}

	if (Input::GetIsPadTrigger(R_SHOULDER)) {
		Shot();
	}

	shotEnergy_ = std::clamp(shotEnergy_, 0.0f, 1.0f);


	BaseGameObject::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Draw() const {
	BaseGameObject::Draw();
	//test_->Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　攻撃関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Shot() {
	if (shotEnergy_ <= 0.0f) {
		isShot_ = false;
		return;
	}

	isShot_ = true;
	Vector3 dire = (reticlrPos_ - worldPos_).Normalize();
	shotQuaternion_ = Quaternion::LookAt(worldPos_, reticlrPos_, Vector3::FORWARD());
	shotEnergy_ -= GameTimer::DeltaTime();
	shotEnergyRaito_ = shotEnergy_ / 1.0f;

	shotSe_->Play(true, 0.3f, true);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void Player::Debug_Gui() {
	if (ImGui::TreeNode("player")) {
		ImGui::Begin("player");
		transform_->Debug_Gui();
		ImGui::Text("score : %d", score_);
		ImGui::Text("shotEnergyRaito : %f", shotEnergyRaito_);

		ImGui::End();
		ImGui::TreePop();
	}
}
#endif
