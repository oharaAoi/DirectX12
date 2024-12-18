#pragma once
#include "Game/GameEffect/IGameEffect.h"
#include "Game/GameEffect/Missile/MissileExprodeEmitter.h"

class MissileExprode : public IGameEffect {
public:

	MissileExprode();
	~MissileExprode();

	void Init() override;
	void Update(GpuParticle* gpuParticle) override;

	MissileExprodeEmitter* GetEmitter() { return gpuEmitter_.get(); }

private:

	std::unique_ptr<MissileExprodeEmitter> gpuEmitter_;
};

