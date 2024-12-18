#pragma once
#include "Engine/ParticleSystem/GpuEmitter.h"

class MissileExprodeEmitter : public GpuEmitter {
public:

	MissileExprodeEmitter();
	~MissileExprodeEmitter();

	void Init() override;
	void Update()override;
	void BindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex)override;

};

