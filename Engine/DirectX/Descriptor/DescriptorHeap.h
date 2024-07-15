#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include "dxgidebug.h"
#include <cassert>
#include <wrl.h>
// utilities
#include "DirectXUtils.h"

template<typename T>
using ComPtr = Microsoft::WRL::ComPtr <T>;

class DescriptorHeap {
public:
	/// <summary>
	/// GPUやCPUのアドレスをまとめた関数
	/// </summary>
	struct DescriptorHandles {
		D3D12_CPU_DESCRIPTOR_HANDLE handleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE handleGPU;
	};

public:

	DescriptorHeap(ID3D12Device* device);
	~DescriptorHeap();

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="device"></param>
	void Initialize(ID3D12Device* device);

	/// <summary>
	/// 終了関数
	/// </summary>
	void Finalize();

	/// <summary>
	/// SRVをセットする
	/// </summary>
	/// <param name="commandList"></param>
	void SetSRVHeap(ID3D12GraphicsCommandList* commandList);


public:
	/// <summary>
	/// RTVのHeapを取得する関数
	/// </summary>
	/// <returns></returns>
	ID3D12DescriptorHeap* GetRTVHeap() { return rtvHeap_.Get(); }

	/// <summary>
	///　SRVのHeapを取得する関数
	/// </summary>
	/// <returns></returns>
	ID3D12DescriptorHeap* GetSRVHeap() { return srvHeap_.Get(); }

	/// <summary>
	/// DSVのHeapを取得する関数
	/// </summary>
	/// <returns></returns>
	ID3D12DescriptorHeap* GetDSVHeap() { return dsvHeap_.Get(); }

	void SetUseSrvIndex(const uint32_t& index) { useSrvIndex_ = index; }

	uint32_t GetUseSrvIndex() { return useSrvIndex_; }

private:

	ID3D12Device* device_ = nullptr;

	ComPtr<ID3D12DescriptorHeap> rtvHeap_ = nullptr;
	ComPtr<ID3D12DescriptorHeap> srvHeap_ = nullptr;
	ComPtr<ID3D12DescriptorHeap> dsvHeap_ = nullptr;
	
	uint32_t useSrvIndex_;
};