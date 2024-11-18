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
}

void DownEffect::Update() {
	if (lifeTime_ < 0.0f) {
		isAlive_ = false;
	}

	Vector3 pos = transform_->GetTranslation();
	pos += velocity_;
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
}
