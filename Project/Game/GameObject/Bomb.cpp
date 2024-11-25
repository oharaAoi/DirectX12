#include "Bomb.h"

Bomb::Bomb() {}
Bomb::~Bomb() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓ 終了処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Bomb::Finalize() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Bomb::Init() {
	BaseGameObject::Init();
	SetObject("bomb.obj");

	// Colliderの生成
	SetMeshCollider("bomb");
	meshCollider_->SetCollisionEnter([this](MeshCollider& other) {OnCollisionEnter(other); });
	meshCollider_->SetCollisionStay([this](MeshCollider& other) {OnCollisionStay(other); });
	meshCollider_->SetCollisionExit([this](MeshCollider& other) {OnCollisionExit(other); });

	isAlive_ = true;

	acceleration_ = Vector3::ZERO();

	pFunc_ = &Bomb::NormalMove;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Bomb::Update() {

	ExecuteFunc(pFunc_);

	BaseGameObject::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Bomb::Draw() const {
	BaseGameObject::Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　出現させる
//////////////////////////////////////////////////////////////////////////////////////////////////

void Bomb::Pop(const Vector3& pos, const Vector3& acceleration) {
	transform_->SetTranslaion(pos);
	acceleration_ = acceleration;
	velocity_ = Vector3::ZERO();
}

void Bomb::NormalMove() {
	velocity_ += acceleration_ * GameTimer::DeltaTime();
	Vector3 pos = transform_->GetTranslation();
	pos += velocity_;
	transform_->SetTranslaion(pos);
}

void Bomb::PullwMove() {
	Vector3 position = wireTipPos_;
	transform_->SetTranslaion({ position.x,transform_->GetTranslation().y,position.z });
}

#ifdef _DEBUG
void Bomb::Debug_Draw() {
	meshCollider_->Draw();
}
#endif

void Bomb::SetPlayerInfoHeldByBomb(bool isNeglect, bool isPull, const Vector3& wireTipPos) {
	playerNeglect_ = isNeglect;
	isPlayerPull_ = isPull;
	if (isPulling_ && isPull) {
		pFunc_ = &Bomb::PullwMove;
		wireTipPos_ = wireTipPos;
	}
	else {
		pFunc_ = &Bomb::NormalMove;
		isPulling_ = false;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　衝突判定用処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Bomb::OnCollisionEnter([[maybe_unused]] MeshCollider& other) {
	if (other.GetTag() == "right_hand" || other.GetTag() == "left_hand") {
		if (other.GetSubTag() != "wait_attack") {
			isAlive_ = false;
		}
	} else if (other.GetTag() == "field") {
		acceleration_ = Vector3::ZERO();
		velocity_ = Vector3::ZERO();
	}
	if (other.GetTag() == "notCatchWireTip") {
		if (!playerNeglect_) {
			isPulling_ = true;
		}
	}

}

void Bomb::OnCollisionStay([[maybe_unused]] MeshCollider& other) {
	if (other.GetTag() == "notCatchWireTip") {
		if (!playerNeglect_) {
			isPulling_ = true;
		}
	}
}

void Bomb::OnCollisionExit([[maybe_unused]] MeshCollider& other) {
}
