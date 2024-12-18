#pragma once
#include "Game/GameEffect/IGameEffect.h"
#include "Game/GameEffect/Missile/MissileHitExprodeEmitter.h"

class MissileHitExprode : public IGameEffect {
public:

	MissileHitExprode();
	~MissileHitExprode();

	void Init() override;
	void Update(GpuParticle* gpuParticle) override;

	MissileHitExprodeEmitter* GetEmitter() { return gpuEmitter_.get(); }

private:

	std::unique_ptr<MissileHitExprodeEmitter> gpuEmitter_;
};

