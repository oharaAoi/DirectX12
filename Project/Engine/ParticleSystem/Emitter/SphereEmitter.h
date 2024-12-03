#pragma once
#include "Engine/ParticleSystem/Emitter/GpuEmitter.h"

class SphereEmitter : public GpuEmitter {
public:

	struct Emitter {
		Vector3 translate;		// 位置
		float radius;			// 射出半径
		uint32_t count;			// 射出数
		float frequency;		// 射出間隔
		float frequencyTime;	// 時間調整用
		int emit;				// 射出許可
	};

public:

	SphereEmitter();
	~SphereEmitter();

	void Init() override;
	void Update() override;
	void BindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex) override;

	void DrawShape(const Matrix4x4& viewProjectionMat) override;

#ifdef _DEBUG
	void Debug_Gui() override;
#endif


private:

	Emitter* sphereEmitter_;

};

