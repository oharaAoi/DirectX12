#include "PlayerMoveState.h"
#include "Game/GameObject/Player/Player.h"
#include "Engine/Utilities/AdjustmentItem.h"

PlayerMoveState::~PlayerMoveState() {
}

void PlayerMoveState::Init() {
	stateName_ = "playerMoveState";

	work_.speed = 6.0f;

	information_.FromJson(AdjustmentItem::GetData(stateName_, stateName_));
	pPlayer_->GetAnimetor()->TransitionAnimation(information_.animationName);
}

void PlayerMoveState::Update() {
	Jump();

	Move();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　jumpする
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerMoveState::Jump() {
	// ジャンプして入なかったら早期リターン
	if (!pPlayer_->GetIsJump()) {
		return;
	}
	// 速度加速度を取得
	velocity_ = pPlayer_->GetVelocity();
	acceleration_ = pPlayer_->GetAcceleration();
	// 移動する
	Vector3 translate = pPlayer_->GetTransform()->GetTranslation();
	velocity_ += acceleration_ * GameTimer::DeltaTime();
	translate += velocity_ * GameTimer::DeltaTime();

	// 限界の範囲を指定
	if (translate.y <= 0.0f) {
		translate.y = 0.0f;
		pPlayer_->SetIsJump(false);
	}
	// 実際に代入する
	pPlayer_->GetTransform()->SetTranslaion((translate));

	// 速度加速度を更新する
	pPlayer_->SetVelocity(velocity_);
	pPlayer_->SetAcceleration(acceleration_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　移動する
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerMoveState::Move() {
	isMoving_ = false;
	
	Vector2 inputJoyStateL = Input::GetLeftJoyStick();
	Vector3 velocity = { inputJoyStateL.x,0.0f,inputJoyStateL.y, };

	// スティックの押し込みが閾値を超えていたら移動可能にする
	if (velocity.Length() > kThreshold) {
		isMoving_ = true;
	}

	// 閾値を超えていたら移動
	if (isMoving_) {
		// 移動ベクトルをカメラの角度だけ回転する
		Matrix4x4 cameraRotate = pPlayer_->GetFollowCamera()->GetRotateMat();
		Vector3 direction = TransformNormal(velocity, cameraRotate);
		velocity = direction.Normalize() * work_.speed;

		Vector3 translate = pPlayer_->GetTransform()->GetTranslation();
		translate += velocity * GameTimer::DeltaTime();

		pPlayer_->GetTransform()->SetTranslaion(translate);

		// playerの向きを移動方向にする
		float targetAngle = std::atan2f(velocity.x, velocity.z);
		pPlayer_->GetTransform()->SetQuaternion(Quaternion::AngleAxis(targetAngle, Vector3::UP()));
	}

	// 状態を編集する状態だったら変化させない
	if (pPlayer_->GetIsStateDebug()) {
		return;
	}

	// defaultに戻す判定の処理
	if (!pPlayer_->GetIsJump()) {
		if (inputJoyStateL.x == 0.0f && inputJoyStateL.y == 0.0f) {
			pPlayer_->SetBehaviorRequest(Behavior::DEFAULT);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　編集処理
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void PlayerMoveState::Debug_Gui() {
	ImGui::Text(stateName_.c_str());
	information_.animationName = pPlayer_->GetAnimetor()->SelectAnimationName();
	ImGui::Text(information_.animationName.c_str());
	if (ImGui::Button("Save")) {
		AdjustmentItem::Save(stateName_, information_.ToJson(stateName_));
	}
}
#endif // _DEBUG