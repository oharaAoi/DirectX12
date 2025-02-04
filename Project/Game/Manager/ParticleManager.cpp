#include "ParticleManager.h"
#include "Engine/Engine.h"

ParticleManager* ParticleManager::GetInstance() {
	static ParticleManager instance;
	return &instance;
}

void ParticleManager::Finalize() {
	hitSparkEffect_->Finalize();
	swordEffect->Finalize();
	enemyDown_->Finalize();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓ 初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void ParticleManager::Init() {
	hitSparkEffect_ = std::make_unique<HitSparkEffect>();
	hitSparkEffect_->Init();

	swordEffect = std::make_unique<SwordEffect>();
	swordEffect->Init();

	enemyDown_ = std::make_unique<EnemyDownEffect>();
	enemyDown_->Init();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓ 更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void ParticleManager::Update() {
	hitSparkEffect_->Update();
	swordEffect->Update();
	enemyDown_->Update();

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓ 描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void ParticleManager::Draw() const {
	ID3D12GraphicsCommandList* commandList = Engine::GetCommandList();
	hitSparkEffect_->Draw(commandList);
	swordEffect->Draw(commandList);
	enemyDown_->Draw(commandList);
}

void ParticleManager::CreateParticle(const ParticlesType& type, 
									 const Vector3& pos,
									 uint32_t createNum) {

	switch (type) {
	case ParticlesType::HitSpark:
		hitSparkEffect_->MakeParticle(pos, createNum);
		break;
	case ParticlesType::SwordEffect:
		swordEffect->MakeParticle(pos, createNum);
		break;
	case ParticlesType::EnemyDown:
		enemyDown_->MakeParticle(pos, createNum);
		break;
	default:
		break;
	}
}
