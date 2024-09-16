#pragma once
// DirectX
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <dxgidebug.h>
// utilities
#include "Engine/Utilities/Convert.h"
#include "Engine/Utilities/DirectXUtils.h"

template<typename T>
using ComPtr = Microsoft::WRL::ComPtr <T>;

class DirectXDevice {
public:

	DirectXDevice(IDXGIAdapter4* useAdapter);
	~DirectXDevice();

	void Initialize(IDXGIAdapter4* useAdapter);

	void Finalize();

public:

	ID3D12Device* GetDevice() { return device_.Get(); }

private:

	ComPtr<ID3D12Device> device_ = nullptr;

};