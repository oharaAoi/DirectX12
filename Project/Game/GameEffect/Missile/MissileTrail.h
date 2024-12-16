#pragma once
#include <memory>
#include "Engine/ParticleSystem/GpuParticle.h"
#include "Game/GameEffect/Missile/MissileTrailEmitter.h"

class MissileTrail {
public:

	MissileTrail();
	~MissileTrail();

	void Init();
	void Update();
	void Draw() const;

	void Dispatch(ID3D12GraphicsCommandList* commandList, const Vector3& group);

private:

	std::unique_ptr<GpuParticle> gpuParticle_;
	std::unique_ptr<MissileTrailEmitter> gpuEmitter_;

};

