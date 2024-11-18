#include "DownEffect.h"

DownEffect::DownEffect() {
}

DownEffect::~DownEffect() {
}

void DownEffect::Finalize() {
}

void DownEffect::Init() {
	BaseGameObject::Init();
	SetObject("effectStar.obj");

	SetIsLighting(false);
}

void DownEffect::Update() {
	lifeTime_ -= GameTimer::DeltaTime();

	if (lifeTime_ < 0.0f) {
		isAlive_ = false;
	}

	Vector3 pos = transform_->GetTranslation();
	pos += (velocity_ * speed_ * GameTimer::DeltaTime());
	transform_->SetTranslaion(pos);

	BaseGameObject::Update();
}

void DownEffect::Draw() const {
	BaseGameObject::Draw();
}

void DownEffect::SetMove(const Vector3& pos, const Vector3& velocity, float lifeTime) {
	velocity_ = velocity;
	lifeTime_ = lifeTime;
	isAlive_ = true;

	transform_->SetTranslaion(pos);
}
