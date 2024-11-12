#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include "dxgidebug.h"
#include <cassert>
#include <wrl.h>
#include <list>
#include "Engine/Utilities/DirectXUtils.h"

#include "Engine/DirectX/Descriptor/DescriptorSize.h"

class DescriptorAllocator;

template<typename T>
using ComPtr = Microsoft::WRL::ComPtr <T>;

enum DescriptorHeapType {
	TYPE_RTV,
	TYPE_SRV,
	TYPE_DSV
};

class DescriptorHeap {
public:
	/// <summary>
	/// GPUやCPUのアドレスをまとめた関数
	/// </summary>
	struct DescriptorHandles {
		D3D12_CPU_DESCRIPTOR_HANDLE handleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE handleGPU;
		int assignIndex_;
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

	/// <summary>
	/// Viewを作成する際のアドレスを取得するための関数
	/// </summary>
	/// <param name="type">descriptorの種類</param>
	/// <param name="descriptorSize">サイズ</param>
	/// <returns></returns>
	DescriptorHandles GetDescriptorHandle(const DescriptorHeapType& type);

	/// <summary>
	/// リストに入っているindexをheapから解放する
	/// </summary>
	void FreeList();

	/// <summary>
	/// リストに追加する
	/// </summary>
	/// <param name="index"></param>
	static void AddFreeSrvList(int index);

public:

	/// <summary>
	/// descriptorSizeを得る
	/// </summary>
	/// <returns></returns>
	DescriptorSize* GetDescriptorSize() { return descriptorSize_.get(); }

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

	DescriptorHandles AllocateSRV();
	DescriptorHandles AllocateRTV();
	DescriptorHandles AllocateDSV();

	void FreeSRV(uint32_t index);
	void FreeRTV(uint32_t index);
	void FreeDSV(uint32_t index);


private:

	ID3D12Device* device_ = nullptr;

	std::unique_ptr<DescriptorSize> descriptorSize_;

	ComPtr<ID3D12DescriptorHeap> rtvHeap_ = nullptr;
	ComPtr<ID3D12DescriptorHeap> srvHeap_ = nullptr;
	ComPtr<ID3D12DescriptorHeap> dsvHeap_ = nullptr;

	std::unique_ptr<DescriptorAllocator> srvAllocator_;
	std::unique_ptr<DescriptorAllocator> rtvAllocator_;
	std::unique_ptr<DescriptorAllocator> dsvAllocator_;
	
	int32_t useSrvIndex_;
	int32_t useDsvIndex_;
	int32_t useRtvIndex_;

	static std::list<int> freeSrvList_;
};