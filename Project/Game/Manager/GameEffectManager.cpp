#include "GameEffectManager.h"
#include "Engine/Engine.h"

GameEffectManager::~GameEffectManager() {
}

GameEffectManager* GameEffectManager::GetInstance() {
	static GameEffectManager instance;
	return &instance;
}

void GameEffectManager::Finalize() {
	effectList_.clear();
	newEffectList_.clear();
	gpuParticle_.reset();
}
//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameEffectManager::Init() {
	gpuParticle_ = std::make_unique<GpuParticle>();
	gpuParticle_->Init("cube.obj", 1024 * 2);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
/////////////////////////////////////////////////////////////////////////////////////////////////

void GameEffectManager::Update() {
	effectList_.remove_if([](auto& effect) {
		return effect->GetIsDead();
						  });

	newEffectList_.remove_if([](auto& effect) {
		return effect->GetIsDead();
						  });


	for (auto& effect : effectList_) {
		effect->Update(gpuParticle_.get());
	}

	for (auto& effect : newEffectList_) {
		effect->Update(gpuParticle_.get());
	}

	gpuParticle_->Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameEffectManager::Draw() {
	Engine::SetPipeline(PipelineType::ParticlePipeline);
	gpuParticle_->Draw(Engine::GetCommandList());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
/////////////////////////////////////////////////////////////////////////////////////////////////

void GameEffectManager::AddEffect(IGameEffect* gameEffect, const Vector3& pos, WorldTransform* worldTransform) {
	GetInstance()->AddEffectList(gameEffect, pos, worldTransform);
}

void GameEffectManager::AddNewEffect(std::unique_ptr<IGameEffect> gameEffect, const Vector3& pos, WorldTransform* worldTransform) {
	GetInstance()->AddNewEffectList(std::move(gameEffect), pos, worldTransform);

}

//================================================================================================//
// 
//　private関数
// 
//================================================================================================//

void GameEffectManager::AddEffectList(IGameEffect* gameEffect, const Vector3& pos, WorldTransform* worldTransform) {
	auto& newEffect = effectList_.emplace_back(gameEffect);
	newEffect->SetEmitPos(pos);
	newEffect->Init();
	if (worldTransform != nullptr) {
		newEffect->SetParentWorldTransform(worldTransform);
	}
}

void GameEffectManager::AddNewEffectList(std::unique_ptr<IGameEffect> gameEffect, const Vector3& pos, WorldTransform* worldTransform) {
	auto& newEffect = newEffectList_.emplace_back(std::move(gameEffect));
	newEffect->SetEmitPos(pos);
	newEffect->Init();
	if (worldTransform != nullptr) {
		newEffect->SetParentWorldTransform(worldTransform);
	}
}
