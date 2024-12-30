#pragma once
#include "Engine/System/ParticleSystem/Emitter/GpuEmitter.h"

class SphereEmitter : public GpuEmitter {
public:

	struct Emitter {
		float radius;			// 射出半径
	};

public:

	SphereEmitter();
	~SphereEmitter();

	void Init() override;
	void Update() override;
	void BindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex) override;

	void DrawShape(const Matrix4x4& viewProjectionMat) override;

	void Save() override;

	void Load() override;

#ifdef _DEBUG
	void Debug_Gui() override;
#endif


private:

	ComPtr<ID3D12Resource> sphereEmitterBuffer_;
	Emitter* emitter_;

};

