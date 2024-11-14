#pragma once
#include <memory>
#include "Engine/Engine.h"
#include "Engine/ParticleSystem/GpuParticle.h"
#include "Engine/ParticleSystem/GpuEmitter.h"

class GpuEffect {
public:

	GpuEffect();
	~GpuEffect();

	void Init();
	void Update();
	void Draw() const;

	void Dispatch(ID3D12GraphicsCommandList* commandList, const Vector3& group);

private:

	std::unique_ptr<GpuParticle> gpuParticle_;
	std::unique_ptr<GpuEmitter> gpuEmitter_;

};

