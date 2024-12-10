#include "PlayerMoveState.h"
#include "Game/GameObject/Player/Player.h"

PlayerMoveState::~PlayerMoveState() {
}

void PlayerMoveState::Init() {
	stateName_ = "playerMoveState";
}

void PlayerMoveState::Update() {
	Jump();

	Move();

	preIsJump_ = pPlayer_->GetIsJump();
}

void PlayerMoveState::Jump() {
	if (!pPlayer_->GetIsJump()) {
		return;
	}

	if (preIsJump_ == false) {
		velocity_.y = 6.0f;
		acceleration_.y = -9.8f;
	}
	Vector3 translate = pPlayer_->GetTransform()->GetTranslation();

	velocity_ += acceleration_ * GameTimer::DeltaTime();
	translate += velocity_ * GameTimer::DeltaTime();

	if (translate.y <= 0.0f) {
		translate.y = 0.0f;
		pPlayer_->SetIsJump(false);
	}

	pPlayer_->GetTransform()->SetTranslaion((translate));

}

void PlayerMoveState::Move() {
	const float threshold = 0.7f;
	const float speed = 6.0f;
	//float targetAngle = 0;
	bool isMoveing = false;

	Vector2 inputJoyStateL = Input::GetLeftJoyStick();

	velocity_ = { inputJoyStateL.x,0.0f,inputJoyStateL.y, };

	// スティックの押し込みが閾値を超えていたら移動可能にする
	if (velocity_.Length() > threshold) {
		isMoveing = true;
	}

	if (isMoveing) {
		// 移動ベクトルをカメラの角度だけ回転する
		Matrix4x4 cameraRotate = pPlayer_->GetFollowCamera()->GetRotateMat();
		velocity_ = TransformNormal(velocity_, cameraRotate);

		velocity_ = velocity_.Normalize() * speed;

		Vector3 translate = pPlayer_->GetTransform()->GetTranslation();
		translate += velocity_ * GameTimer::DeltaTime();

		pPlayer_->GetTransform()->SetTranslaion((translate));
	}
}

#ifdef _DEBUG
void PlayerMoveState::Debug_Gui() {

}
#endif // _DEBUG