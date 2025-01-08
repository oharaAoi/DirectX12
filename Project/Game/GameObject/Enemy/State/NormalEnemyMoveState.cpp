#include "NormalEnemyMoveState.h"
#include "Game/GameObject/Enemy/NormalEnemy.h"

NormalEnemyMoveState::~NormalEnemyMoveState() {
}

void NormalEnemyMoveState::Init() {
	stateName_ = "move";

	pEnemy_->GetAnimetor()->TransitionAnimation("move", 0.5f);
}

void NormalEnemyMoveState::Update() {
	Move();

	if (distance_.Length() < 5.0f) {
		pEnemy_->SetBehaviorRequest(EnemyBehavior::ATTACK);
	}
}

void NormalEnemyMoveState::Move() {
	Vector3 translate = pEnemy_->GetTransform()->GetTranslation();
	// 計算
	distance_ = pEnemy_->GetPlayerPos() - pEnemy_->GetTransform()->GetTranslation();
	velocity_ = distance_.Normalize();
	// 更新
	translate += velocity_ * GameTimer::DeltaTime();
	ConstrainToField(translate);
	pEnemy_->GetTransform()->SetTranslaion(translate);

	// 対象の方向を向ける
	float angle = std::atan2f(distance_.x, distance_.z);
	Quaternion lookRotate = Quaternion::AngleAxis(angle, Vector3::UP());
	pEnemy_->GetTransform()->SetQuaternion(Quaternion::Slerp(pEnemy_->GetTransform()->GetQuaternion(), lookRotate, 0.1f));
}

void NormalEnemyMoveState::ConstrainToField(Vector3& translate) {
	Vector3 distance = (translate - Vector3(0, translate.y, 0)).Normalize();
	// 中心からの長さ
	float lenght = (translate - Vector3(0, translate.y, 0)).Length();
	if (lenght > 65.0f) {
		distance = distance * 65.0f;
		translate = { distance.x, translate.y, distance.z };
	}
}

#ifdef _DEBUG
void NormalEnemyMoveState::Debug_Gui() {
}
#endif // _DEBUG