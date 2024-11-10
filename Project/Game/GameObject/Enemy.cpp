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

	meshCollider_ = std::make_unique<MeshCollider>();
	meshCollider_->Init(model_->GetMesh(0));

	meshCollider_->SetTag("enemy");
	meshCollider_->SetOnCollisionCallBack([this](MeshCollider& other) {
		this->OnCollision(other);
										  });

	isMove_ = false;
	isAlive_ = true;

	speed_ = 2.0f;

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy::Update() {
	if (isMove_) {
		Vector3 translate = transform_->GetTranslation();
		translate += velocity_ * speed_;
		transform_->SetTranslaion(translate);
	}

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

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Modelの変更
//////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy::ChangeModel() {
	switch (enemyType_) {
	case EnemyType::STAR:
		SetObject("star.obj");
		break;
	case EnemyType::JET:
		SetObject("star.obj");
		break;
	default:
		break;
	}
}

#ifdef _DEBUG
void Enemy::Debug_Gui() {
	Vector3 translate = transform_->GetTranslation();
	// 位置・速度
	ImGui::DragFloat3("newPos", &translate.x, 0.1f);
	ImGui::DragFloat3("newVelocity", &velocity_.x, 0.1f);
	// typeの変更
	if (ImGui::RadioButton("STAR", enemyType_ == EnemyType::STAR)) {
		enemyType_ = EnemyType::STAR;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("JET", enemyType_ == EnemyType::JET)) {
		enemyType_ = EnemyType::JET;
	}

	// 前のtypeを取得
	if (enemyType_ != preEnemyType_) {
		ChangeModel();
	}

	preEnemyType_ = enemyType_;
	transform_->SetTranslaion(translate);
}
#endif

const Vector3 Enemy::GetWorldPos() const {
	return Transform(Vector3::ZERO(), transform_->GetWorldMatrix());
}
