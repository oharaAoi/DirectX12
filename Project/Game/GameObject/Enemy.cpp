#include "Enemy.h"

Enemy::Enemy() {
}

Enemy::~Enemy() {
}

void Enemy::Finalize() {
}
//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy::Init() {
	BaseGameObject::Init();
	SetObject("star.obj");

	isAlive_ = true;

	meshCollider_ = std::make_unique<MeshCollider>();
	meshCollider_->Init(model_->GetMesh(0));

	meshCollider_->SetTag("enemy");
	meshCollider_->SetOnCollisionCallBack([this](MeshCollider& other) {
		this->OnCollision(other);
										  });
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy::Update() {
	BaseGameObject::Update();

	meshCollider_->Update(transform_.get(), Vector3::ZERO());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy::Draw() const {
	Engine::SetPipeline(PipelineType::NormalPipeline);
	BaseGameObject::Draw();

	Engine::SetPipeline(PipelineType::PrimitivePipeline);
	meshCollider_->Draw();
}

void Enemy::OnCollision(MeshCollider& other) {
	if (other.GetTag() == "playerBullet") {
		isAlive_ = false;
	}
}

const Vector3 Enemy::GetWorldPos() const {
	return Transform(Vector3::ZERO(), transform_->GetWorldMatrix());
}
