#include "NormalEnemyDefaultState.h"
#include "Game/GameObject/Enemy/NormalEnemy.h"
#include "Engine/Math/MyRandom.h"

NormalEnemyDefaultState::~NormalEnemyDefaultState() {
}

void NormalEnemyDefaultState::Init() {
	stateName_ = "default";

	pEnemy_->GetAnimetor()->TransitionAnimation("default", 0.5f);

	time_ = 0.0;

	velocity_ = Vector3{ RandomFloat(-1, 1),0.0f, RandomFloat(-1, 1) }.Normalize();
}

void NormalEnemyDefaultState::Update() {
	Move();

	time_ += GameTimer::DeltaTime();
	if (time_ >= changeTime_) {
		velocity_ = Vector3{ RandomFloat(-1, 1),0.0f, RandomFloat(-1, 1) }.Normalize();
		time_ = 0.0f;
	}

	if ((pEnemy_->GetTransform()->GetTranslation() - pEnemy_->GetPlayerPos()).Length() < 10.0f) {
		pEnemy_->SetBehaviorRequest(EnemyBehavior::MOVE);
	}
}

void NormalEnemyDefaultState::Move() {
	Vector3 translate = pEnemy_->GetTransform()->GetTranslation();
	// 更新
	translate += velocity_ * GameTimer::DeltaTime();
	ConstrainToField(translate);
	pEnemy_->GetTransform()->SetTranslaion(translate);

	// 対象の方向を向ける
	float angle = std::atan2f(velocity_.x, velocity_.z);
	Quaternion lookRotate = Quaternion::AngleAxis(angle, Vector3::UP());
	pEnemy_->GetTransform()->SetQuaternion(Quaternion::Slerp(pEnemy_->GetTransform()->GetQuaternion(), lookRotate, 0.1f));
}

void NormalEnemyDefaultState::ConstrainToField(Vector3& translate) {
	Vector3 distance = (translate - Vector3(0, translate.y, 0)).Normalize();
	// 中心からの長さ
	float lenght = (translate - Vector3(0, translate.y, 0)).Length();
	if (lenght > 65.0f) {
		distance = distance * 65.0f;
		translate = { distance.x, translate.y, distance.z };
	}
}

#ifdef _DEBUG
void NormalEnemyDefaultState::Debug_Gui() {
}
#endif // _DEBUG