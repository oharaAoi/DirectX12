#pragma once
#include "Engine/ParticleSystem/GpuEmitter.h"

class MissileTrailEmitter : public GpuEmitter {
public:

	MissileTrailEmitter();
	~MissileTrailEmitter();

	void Init() override;
	void Update()override;
	void BindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex)override;

private:

};

