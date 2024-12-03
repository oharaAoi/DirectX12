#pragma once
#include "Engine/ParticleSystem/Emitter/GpuEmitter.h"

class ConeEmitter : public GpuEmitter {
public:

	struct Emitter {
		Vector3 translate;		// 位置
		float pad;
		float radius;			// 射出半径
		float angle;			// 円錐の角度
		float height;			// 円錐の高さ
		uint32_t count;			// 射出数
		float frequency;		// 射出間隔
		float frequencyTime;	// 時間調整用
		int emit;				// 射出許可
	};

public:

	ConeEmitter();
	~ConeEmitter();

	void Init() override;
	void Update() override;
	void BindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex) override;

	void DrawShape(const Matrix4x4& viewProjectionMat) override;

#ifdef _DEBUG
	void Debug_Gui() override;
#endif


private:

	Emitter* emitter_;

};

