#pragma once
#include <Windows.h>
#include <cstdint>
#include <d3d12.h>
#include <wrl.h>
// DirectX
#include "Engine/Utilities/DirectXUtils.h"
// math
#include "Engine/Math/MyMath.h"
#include "Engine/Manager/ImGuiManager.h"

class BaseLight {
public:

	BaseLight() = default;
	virtual ~BaseLight() = default;

	virtual void Init(ID3D12Device* device, const size_t& size);

	virtual void Finalize();

	virtual void Update();

	virtual void Draw(ID3D12GraphicsCommandList* commandList, const uint32_t& rootParameterIndex);

protected:

	Microsoft::WRL::ComPtr<ID3D12Resource>lightBuffer_;

};

