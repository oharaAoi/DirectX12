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

	ID3D12DescriptorHeap* GetRTVHeap() { return rtvHeap_.Get(); }

	ID3D12DescriptorHeap* GetSRVHeap() { return srvHeap_.Get(); }

	ID3D12DescriptorHeap* GetDSVHeap() { return dsvHeap_.Get(); }

	void SetUseSrvIndex(const uint32_t& index) { useSrvIndex_ = index; }

	uint32_t GetUseSrvIndex() { return useSrvIndex_; }

private:

	ID3D12Device* device_ = nullptr;

	Comptr<ID3D12DescriptorHeap> rtvHeap_ = nullptr;
	Comptr<ID3D12DescriptorHeap> srvHeap_ = nullptr;
	Comptr<ID3D12DescriptorHeap> dsvHeap_ = nullptr;

	uint32_t useSrvIndex_;
};