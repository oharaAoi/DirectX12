#pragma once
// DirectX
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <dxgidebug.h>
// utilities
#include "Convert.h"
#include "DirectXUtils.h"

template<typename T>
using Comptr = Microsoft::WRL::ComPtr <T>;

class DirectXDevice {
public:

	DirectXDevice(IDXGIAdapter4* useAdapter);
	~DirectXDevice();

	void Initialize(IDXGIAdapter4* useAdapter);

	void Finalize();

public:

	ID3D12Device* GetDevice() { return device_.Get(); }

private:

	Comptr<ID3D12Device> device_ = nullptr;

};