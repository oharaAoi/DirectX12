#pragma once
#include "Game/GameEffect/IGameEffect.h"
#include "Game/GameEffect/Missile/MissileTrailEmitter.h"

class MissileTrail : public IGameEffect {
public:

	MissileTrail();
	~MissileTrail();

	void Init() override;
	void Update(GpuParticle* gpuParticle) override;
	
	MissileTrailEmitter* GetEmitter() { return gpuEmitter_.get(); }

private:
	
	std::unique_ptr<MissileTrailEmitter> gpuEmitter_;
};

