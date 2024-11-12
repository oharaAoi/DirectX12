#include "Boss.h"

Boss::Boss() {
}

Boss::~Boss() {
}

void Boss::Finalize() {
}

void Boss::Init() {
	BaseGameObject::Init();
	SetObject("boss.obj");

	meshCollider_ = std::make_unique<MeshCollider>();
	meshCollider_->Init(model_->GetMesh(0));

	meshCollider_->SetTag("boss");
	meshCollider_->SetOnCollisionCallBack([this](MeshCollider& other) {
		this->OnCollision(other);
										  });

	controlPoints_ = {
		{-62.0f, 40.8f, -134.0f},
		{-36.7f, 47.0f, -134.0f},
		{-10.7f, 47.0f, -91.0f},
		{-32.7f, 47.0f, -85.0f},
		{-7.7f, 47.0f, -133.0f},
		{-7.7f, 47.0f, -97.5f},
		{-31.7f, 47.0f, -132.5f},
		{-10.7f, 47.0f, -132.5f},
		{9.7f, 47.0f, -84.5f}
	};

	SetIsLighting(false);

	isAlive_ = true;
}

void Boss::Update() {
	if (!isAlive_) {
		return;
	}

	if (!isMove_) {
		return;
	}

	const size_t segmentCount = 10;
	// 点からSpline曲線を引く
	t_ += (1.0f / static_cast<float>(segmentCount)) * (GameTimer::DeltaTime());
	if (t_ >= 1.0f) {
		t_= 1.0f;
		isFinish_ = true;
	}

	Vector3 pos = CatmullRomPosition(controlPoints_, t_);
	transform_->SetTranslaion(pos);

	BaseGameObject::Update();

	meshCollider_->Update(transform_.get(), Vector3::ZERO());
}

void Boss::Draw() const {
	if (!isAlive_) {
		return;
	}

	if (!isMove_) {
		return;
	}

	BaseGameObject::Draw();
}

void Boss::OnCollision(MeshCollider& other) {
	if (!isMove_) {
		return;
	}

	if (!isAlive_) {
		return;
	}

	if (other.GetTag() == "playerBullet") {
		hitedTime_ += GameTimer::DeltaTime();

		if (hitedTime_ > 7.0f) {
			isAlive_ = false;
			isFinish_ = true;
			Player::AddScore(3000);
			KnockDownEnemy::SetObj("boss.obj", 3000);
			AudioPlayer::SinglShotPlay("hit.mp3", 0.3f);
		}
	}
}

#ifdef _DEBUG
void Boss::Debug_Gui() {
	ImGui::Begin("Boss");
	Vector3 pos = transform_->GetTranslation();
	ImGui::DragFloat3("pos", &pos.x, 0.1f);
	transform_->SetTranslaion(pos);

	ImGui::End();
}
#endif