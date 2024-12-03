#pragma once
#include <string>
#include "Engine/Math/Vector3.h"
#include "Engine/Utilities/DirectXUtils.h"

/// <summary>
/// Emitterの基底クラス
/// </summary>
class GpuEmitter {
public:

	struct PerFrame {
		float time;
		float deltaTime;
	};

public:

	GpuEmitter() = default;
	virtual ~GpuEmitter() = default;

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void BindCmdList(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex) = 0;

	virtual void DrawShape(const Matrix4x4& viewProjectionMat) = 0;

#ifdef _DEBUG
	virtual void Debug_Gui() = 0;
#endif

protected:

	ComPtr<ID3D12Resource> emitterBuffer_;
	
	ComPtr<ID3D12Resource> perFrameBuffer_;
	PerFrame* perFrame_;

};

