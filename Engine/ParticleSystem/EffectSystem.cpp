#include "EffectSystem.h"

EffectSystem::~EffectSystem() {}

EffectSystem* EffectSystem::GetInstacne() {
	static EffectSystem instance;
	return &instance;
}

void EffectSystem::Init() {
	particleField_ = std::make_unique<ParticleField>();
	particleField_->Init();
	/*particleField_->SetParticle(particle_.get());*/

	CreateEffect();
}

void EffectSystem::Finalize() {
	for (std::list<EffectData>::iterator effectDataListIter = effectList_.begin(); effectDataListIter != effectList_.end();) {
		for (std::list<std::unique_ptr<BaseEffect>>::iterator effectListIter = effectDataListIter->effectList.begin();
			 effectListIter != effectDataListIter->effectList.end();) {
			(*effectListIter)->Finalize();

			++effectListIter;
		}
		++effectDataListIter;
	}
}

void EffectSystem::Update() {
	for (std::list<EffectData>::iterator effectDataListIter = effectList_.begin(); effectDataListIter != effectList_.end();) {
		// -------------------------------------------------
		// ↓ エミッターの更新
		// -------------------------------------------------
		for (std::list<std::unique_ptr<Emitter>>::iterator emitterListIter = effectDataListIter->emitterList.begin(); emitterListIter != effectDataListIter->emitterList.end();) {
			(*emitterListIter)->Update();
			++emitterListIter;
		}

		// -------------------------------------------------
		// ↓ effectの更新
		// -------------------------------------------------
		for (std::list<std::unique_ptr<BaseEffect>>::iterator effectListIter = effectDataListIter->effectList.begin(); effectListIter != effectDataListIter->effectList.end();) {
			(*effectListIter)->SetCameraMatrix(cameraMat_);
			(*effectListIter)->Update(viewMat_, projectionMat_);
			++effectListIter;
		}

		++effectDataListIter;
	}

	// -------------------------------------------------
	// ↓ 当たり判定の実装
	// -------------------------------------------------
	//particleField_->Update();

	// -------------------------------------------------
	// ↓ ImGuiの描画
	// -------------------------------------------------
	//particle_->ImGuiDraw();
}

void EffectSystem::Draw() {
	for (std::list<EffectData>::iterator effectDataListIter = effectList_.begin(); effectDataListIter != effectList_.end();) {
		// -------------------------------------------------
		// ↓ effectの更新
		// -------------------------------------------------
		for (std::list<std::unique_ptr<BaseEffect>>::iterator effectListIter = effectDataListIter->effectList.begin(); effectListIter != effectDataListIter->effectList.end();) {
			(*effectListIter)->Draw();
			++effectListIter;
		}
		
		// -------------------------------------------------
		// ↓ エミッターの更新
		// -------------------------------------------------
		for (std::list<std::unique_ptr<Emitter>>::iterator emitterListIter = effectDataListIter->emitterList.begin(); emitterListIter != effectDataListIter->emitterList.end();) {
			(*emitterListIter)->Draw(viewMat_ * projectionMat_);
			++emitterListIter;
		}

		++effectDataListIter;
	}
	
	particleField_->Draw(viewMat_ * projectionMat_);
}

void EffectSystem::CreateEffect() {
	EffectData effectData;
	std::unique_ptr<BaseEffect> effect = std::make_unique<BaseEffect>();
	effect->Init("particle.obj", 10);
	std::unique_ptr<Emitter> emitter = std::make_unique<Emitter>(effect.get());
	
	effectData.emitterList.push_back(std::move(emitter));
	effectData.effectList.push_back(std::move(effect));

	effectList_.push_back(std::move(effectData));
}

void EffectSystem::SetViewProjectionMatrix(const Matrix4x4& viewMat, const Matrix4x4& projection) {
	viewMat_ = viewMat;
	projectionMat_ = projection;
}

