#include "PlayerAvoidanceState.h"
#include "Game/GameObject/Player/Player.h"
#include "Engine/Utilities/AdjustmentItem.h"

PlayerAvoidanceState::~PlayerAvoidanceState() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerAvoidanceState::Init() {
	stateName_ = "playerAvoidanceState";
	// 基本の情報の初期化
	information_.FromJson(AdjustmentItem::GetData(stateName_, stateName_));
	// 特有の行動の初期化
	work_.time = 0.0f;
	work_.FromJson(AdjustmentItem::GetData(stateName_, "work"));
	// Animationの設定
	pPlayer_->GetAnimetor()->TransitionAnimation(information_.animationName, 0.1f);
	// 回避する方向を決定する
	RollMove();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerAvoidanceState::Update() {
	pPlayer_->GetTransform()->SetTranslaion(pPlayer_->GetTransform()->GetTranslation() + (velocity_ * GameTimer::DeltaTime()));
	
	/*if (pPlayer_->GetIsStateDebug()) {
		return;
	}*/

	work_.time += GameTimer::DeltaTime();
	if (work_.time >= work_.timeLimit) {
		pPlayer_->SetBehaviorRequest(Behavior::DEFAULT);
		pPlayer_->SetIsAvoidance(false);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　回転しながら移動する方向を決定する
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerAvoidanceState::RollMove() {
	Vector2 inputJoyStateL = Input::GetLeftJoyStick();
	Vector3 velocity = { inputJoyStateL.x,0.0f,inputJoyStateL.y, };

	// スティックの押し込みが閾値を超えていたらスティック方向に移動させて
	// 超えてなかったらそのままの方向に移動
	if (velocity.Length() > 0.7f) {
		// プレイヤーの方向を求める
		Matrix4x4 cameraRotate = pPlayer_->GetFollowCamera()->GetRotateMat();
		Vector3 direction = TransformNormal(velocity, cameraRotate);
		velocity = direction.Normalize();
		// 移動する
		velocity *= work_.avoidanceLength;
		velocity_ = velocity;
		// 角度を算出する
		float targetAngle = std::atan2f(velocity.x, velocity.z);
		pPlayer_->GetTransform()->SetQuaternion(Quaternion::AngleAxis(targetAngle, Vector3::UP()));
	
	} else {
		// playerの現在の回転を元に回避行動をとる
		Vector3 direction = pPlayer_->GetTransform()->GetQuaternion() * Vector3(0.0f, 0.0f, 1.0f);
		direction *= work_.avoidanceLength;
		velocity_ = direction;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　編集
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void PlayerAvoidanceState::Debug_Gui() {
	ImGui::Text(stateName_.c_str());
	
	if (ImGui::TreeNode("Information")) {
		information_.animationName = pPlayer_->GetAnimetor()->SelectAnimationName();
		ImGui::Text(information_.animationName.c_str());
		if (ImGui::Button("Save")) {
			AdjustmentItem::Save(stateName_, information_.ToJson(stateName_));
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("work")) {
		ImGui::DragFloat("timeLimit", &work_.timeLimit, 0.1f);
		ImGui::DragFloat("avoidanceLength", &work_.avoidanceLength, 0.1f);

		if (ImGui::Button("Save")) {
			AdjustmentItem::Save(stateName_, work_.ToJson("work"));
		}
		ImGui::TreePop();
	}
}
#endif // _DEBUG