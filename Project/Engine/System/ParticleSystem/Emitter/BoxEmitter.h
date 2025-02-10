#pragma once
#include "Engine/Lib/Math/MathStructures.h"
#include "Engine/System/ParticleSystem/Emitter/GpuEmitter.h"

class BoxEmitter : public GpuEmitter {
public:

	struct Emitter {
		Vector3 size_;
	};

public:

	BoxEmitter();
	~BoxEmitter();

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

	ComPtr<ID3D12Resource> boxEmitterBuffer_;
	Emitter* emitter_;

	OBB obb_;
};

