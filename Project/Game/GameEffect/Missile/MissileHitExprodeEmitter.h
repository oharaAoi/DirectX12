#pragma once
#include "Engine/ParticleSystem/GpuEmitter.h"

class MissileHitExprodeEmitter : public GpuEmitter {
public:

	MissileHitExprodeEmitter();
	~MissileHitExprodeEmitter();

	void Init() override;
	void Update()override;
	void BindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex)override;
};

