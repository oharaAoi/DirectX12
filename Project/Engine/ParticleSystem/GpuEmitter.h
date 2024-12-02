#pragma once
#include <string>
#include "Engine/Math/Vector3.h"
#include "Engine/Utilities/DirectXUtils.h"

class GpuEmitter {
public:

	struct SphereEmitter {
		Vector3 translate;		// 位置
		float radius;			// 射出半径
		uint32_t count;			// 射出数
		float frequency;		// 射出間隔
		float frequencyTime;	// 時間調整用
		int emit;				// 射出許可
	};

	struct PerFrame {
		float time;
		float deltaTime;
	};

public:

	GpuEmitter();
	~GpuEmitter();

	void Init();
	void Update();
	void BindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex);

	void DrawShape(const Matrix4x4& viewProjectionMat);

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	ComPtr<ID3D12Resource> emitterBuffer_;
	SphereEmitter* sphereEmitter_;

	ComPtr<ID3D12Resource> perFrameBuffer_;
	PerFrame* perFrame_;

};

