#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include "dxgidebug.h"
#include <cassert>
#include <wrl.h>
// utilities
#include "DirectXUtils.h"

template<typename T>
using Comptr = Microsoft::WRL::ComPtr <T>;

// -----------------------------------------------------------
// 使わないか
// -----------------------------------------------------------
class DescriptorHeap {
public:

	DescriptorHeap(ID3D12Device* device);
	~DescriptorHeap();

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="device"></param>
	void Initialize(ID3D12Device* device);

	void Finalize();

public:

	ID3D12DescriptorHeap* GetRTVHeap() { return rtvHeap.Get(); }

private:

	ID3D12Device* device_ = nullptr;

	Comptr<ID3D12DescriptorHeap> rtvHeap = nullptr;

};