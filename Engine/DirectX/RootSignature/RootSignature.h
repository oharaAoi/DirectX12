#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include "dxgidebug.h"
#include <cassert>
#include <wrl.h>
// utilities
#include "Convert.h"
#include "DirectXUtils.h"
#include "Debug.h"

template<typename T>
using Comptr = Microsoft::WRL::ComPtr <T>;

class RootSignature {
public:

	RootSignature(ID3D12Device* device);
	~RootSignature();

	void Initialize(ID3D12Device* device);

	Microsoft::WRL::ComPtr<ID3D12RootSignature> CreateRootSignature();

public:

	ID3D12RootSignature* GetRootSignature() { return rootSignature_.Get(); }

private:

	// rootSignature
	Comptr<ID3D12RootSignature> rootSignature_ = nullptr;

	ID3D12Device* device_ = nullptr;
};