#include "Missile.h"

Missile::Missile() {}
Missile::~Missile() {}

void Missile::Finalize() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Missile::Init() {
	BaseGameObject::Init();
	SetObject("missile.obj");

	SetMeshCollider("missile");
	meshCollider_->SetOwner(this);
	meshCollider_->SetCollisionEnter([this](MeshCollider& other) {OnCollisionEnter(other); });
	meshCollider_->SetCollisionStay([this](MeshCollider& other) {OnCollisionStay(other); });
	meshCollider_->SetCollisionExit([this](MeshCollider& other) {OnCollisionExit(other); });

	isWireCaught_ = false;
	isAlive_ = true;
	isThrowed_ = false;

	moveT_ = 0.0f;
	nextMoveT_ = 0.0f;
	speed_ = 5.0f;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Missile::Update() {
	if (isThrowed_) {
		// 位置の更新
		Vector3 position = transform_->GetTranslation();
		position += velocity_ * GameTimer::DeltaTime();
		transform_->SetTranslaion(position);
		// 回転の更新
		Quaternion rotate = Quaternion::FromDirection(velocity_.Normalize());
		transform_->SetQuaternion(rotate.Normalize());

		BaseGameObject::Update();
		return;
	}

	// ワイヤーに捕まって入なかったら
	if (!isWireCaught_) {
		moveT_ += ((1.0f / static_cast<float>(kDivision_)) * speed_) * (GameTimer::DeltaTime());
		nextMoveT_ = moveT_ + (((1.0f / static_cast<float>(kDivision_)) * speed_) * (GameTimer::DeltaTime()));
		if (nextMoveT_ >= 1.0f) {
			isAlive_ = false;
			return;
		}

		Vector3 pos = CatmullRomPosition(movePoint_, moveT_);
		transform_->SetTranslaion(pos);

		// 方向を合わせる
		Vector3 nextPos = CatmullRomPosition(movePoint_, nextMoveT_);
		Quaternion rotate = Quaternion::LookAt(pos, nextPos);
		transform_->SetQuaternion(rotate);
	}

	BaseGameObject::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Missile::Draw() const {
	BaseGameObject::Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　生成時処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Missile::Pop(const Vector3& targePos, const Vector3& firePos) {
	controlPoint_[0] = firePos;
	controlPoint_[0].y += 5.0f;
	controlPoint_[1] = (firePos - targePos);
	controlPoint_[1].x = RandomFloat(-20.0f, 20.0f);
	controlPoint_[1].y += RandomFloat(20.0f, 25.0f);
	controlPoint_[1].z = targePos.z + 5.0f;
	controlPoint_[2] = targePos;

	// 制御点から動く座標を割り出す
	movePoint_.resize(kDivision_);
	for (uint32_t oi = 0; oi < kDivision_ - 1; oi++) {
		float t = (float)oi / kDivision_;
		float t2 = ((float)oi + 1) / kDivision_;

		// 制御点p0,p1の補完
		Vector3 p0p1 = Lerp(controlPoint_[0], controlPoint_[1], t);
		// 制御点p1,p2の補完
		Vector3 p1p2 = Lerp(controlPoint_[1], controlPoint_[2], t);
		// さらに補完
		Vector3 p = Lerp(p0p1, p1p2, t);

		// 制御点p0,p1の補完
		p0p1 = Lerp(controlPoint_[0], controlPoint_[1], t2);
		// 制御点p1,p2の補完
		p1p2 = Lerp(controlPoint_[1], controlPoint_[2], t2);
		// さらに補完
		movePoint_[oi] = Lerp(p0p1, p1p2, t2);
	}

	movePoint_[49] = controlPoint_[2];
}

void Missile::Debug_Draw() {
	meshCollider_->Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　当たり判定の処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Missile::OnCollisionEnter([[maybe_unused]] MeshCollider& other) {
	if (other.GetTag() == "player") {
		if (meshCollider_->GetTag() == "missile") {
			isAlive_ = false;
		}
		// ワイヤーに当たった時
	} else if (other.GetTag() == "wireTip") {
		isWireCaught_ = true;
		meshCollider_->SetTag("throwMissile");

		// Bossのバリアにぶつかった時
	} else if (other.GetTag() == "boss_barrier") {
		if (meshCollider_->GetTag() == "throwMissile") {
			isAlive_ = false;
		}
	}
}

void Missile::OnCollisionStay([[maybe_unused]] MeshCollider& other) {
}

void Missile::OnCollisionExit([[maybe_unused]] MeshCollider& other) {
}
