#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <wrl.h>
// utilities
#include "Convert.h"
#include "DirectXUtils.h"
// heap
#include "DescriptorHeap.h"
#include "DescriptorSize.h"

template<typename T>
using ComPtr = Microsoft::WRL::ComPtr <T>;

class RenderTarget {
public:

	RenderTarget();
	~RenderTarget();

	void Finalize();

	void Init(ID3D12Device* device, DescriptorHeap* descriptorHeap, IDXGISwapChain4* swapChain);

	/// <summary>
	/// バックバッファとフロントバッファのResource作成
	/// </summary>
	void CreateRenderResource();

	/// <summary>
	/// バックバッファとフロントバッファのViewの作成
	/// </summary>
	void CreateRenderTargetView();

	/// <summary>
	/// オフスクリーン用のResource作成
	/// </summary>
	void CreateOffScreenResource();

	/// <summary>
	/// オフスクリーン用のViewの作成
	/// </summary>
	void CreateOffScreenView();

	/// <summary>
	/// リソースの状態を変更する
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="beforState">変更前の状態</param>
	/// <param name="afterState">変更後の状態</param>
	void ChangeOffScreenResource(ID3D12GraphicsCommandList* commandList, const D3D12_RESOURCE_STATES& beforState, const D3D12_RESOURCE_STATES& afterState);

public:

	ID3D12Resource* GetSwapChainRenderResource(const UINT& indexNum) { return swapChainRenderResource_[indexNum].Get(); }

	ID3D12Resource* GetOffScreenRenderResource() { return offScreenRenderResource_.Get(); }

	DescriptorHeap::DescriptorHandles& GetRtvHandles(const UINT& indexNum) { return rtvHandles_[indexNum]; }

	DescriptorHeap::DescriptorHandles& GetOffScreenHandle() { return offScreenHandle_; }

	DescriptorHeap::DescriptorHandles& GetOffScreenSRVHandle() { return offScreenSRVHandle_; }



private:

	// resource
	// フロントバッファとバックバッファ
	ComPtr<ID3D12Resource> swapChainRenderResource_[2];
	// 画像として書き込むバッファ
	ComPtr<ID3D12Resource> offScreenRenderResource_;

	// rtv
	DescriptorHeap::DescriptorHandles rtvHandles_[2];
	DescriptorHeap::DescriptorHandles offScreenHandle_;
	DescriptorHeap::DescriptorHandles offScreenSRVHandle_;

	ID3D12Device* device_ = nullptr;
	// heap
	DescriptorHeap* dxHeap_ = nullptr;
	// swapChain
	IDXGISwapChain4* swapChain_ = nullptr;
};