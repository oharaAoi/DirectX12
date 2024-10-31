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

	wire_ = std::make_unique<ClutchWire>();
	wire_->Init();
	wire_->GetTransform()->SetParent(transform_->GetWorldMatrix());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Update() {

	Move();
	Clutch();

	BaseGameObject::Update();

	wire_->Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Draw() const {
	BaseGameObject::Draw();
	wire_->Draw();
}

void Player::SetvpvpMatrix(const Matrix4x4& vpvp) {
	vpvpMat_ = vpvp;
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
	if (ImGui::TreeNode("Wire")) {
		ImGui::DragFloat2("Wire", &clutchEnd_.x);
		ImGui::TreePop();
	}
}
#endif

void Player::Move() {
	Vector3 pos = transform_->GetTranslation();
	if (Input::IsPressKey(DIK_A)) {
		pos.x -= moveSpeed_ * GameTimer::DeltaTime();
		if (Input::IsPressMouse(0)) {
			clutchEnd_.x += moveSpeed_ * GameTimer::DeltaTime();
		}
	}
	if (Input::IsPressKey(DIK_D)) {
		pos.x += moveSpeed_ * GameTimer::DeltaTime();
		if (Input::IsPressMouse(0)) {
			clutchEnd_.x -= moveSpeed_ * GameTimer::DeltaTime();
		}
	}

	transform_->SetTranslaion(pos);
}

void Player::Clutch() {

	Vector2 start;
	Vector2 clutchDirection;
	if (!isReturnClutch_) {// 最大まで伸びて、戻る状態じゃない時
		if (Input::IsPressMouse(0)) {
			if (!isStretchClutch_) {
				clutchEnd_ = Input::GetMousePosition();
			}
			Vector3 screenPos = Transform({ 0,0,0 }, transform_->GetWorldMatrix() * vpvpMat_);

			start = { screenPos.x,screenPos.y };
			clutchDirection = (clutchEnd_ - start).Normalize();
			float angle = std::atan2f(clutchDirection.x, clutchDirection.y);

			Quaternion moveRotate = Quaternion::AngleAxis(angle, Vector3::FORWARD());
			Quaternion rotate = wire_->GetTransform()->GetQuaternion();
			wire_->GetTransform()->SetQuaternion(moveRotate);
			isStretchClutch_ = true;
		}
	}
	else {
		Vector3 nowScale = wire_->GetTransform()->GetScale();

		nowScale.y = std::lerp(nowScale.y, 0.0f, returnSpeed_);
		if (nowScale.y < 0.1f) {
			nowScale.y = 0.0f;
			isReturnClutch_ = false;
		}
		wire_->GetTransform()->SetScale(nowScale);
	}


	if (isStretchClutch_) {

		Vector3 nowScale = wire_->GetTransform()->GetScale();
		if (nowScale.y <= maxClutchLength_) {
			nowScale.y += stretchSpeed_ * GameTimer::DeltaTime();
			wire_->GetTransform()->SetScale(nowScale);
		}
		else {
			if (!Input::IsPressMouse(0)) {
				isStretchClutch_ = false;
				isReturnClutch_ = true;
			}
		}

	}



}
