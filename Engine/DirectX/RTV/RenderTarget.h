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

	void Init(ID3D12Device* device, DescriptorHeap* descriptorHeap, DescriptorSize* descriptorSize, IDXGISwapChain4* swapChain);

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
	/// オフスクリーン用のリソースのステートを変更する
	/// </summary>
	void ChangeOffScreenResource(ID3D12GraphicsCommandList* commandList, const D3D12_RESOURCE_STATES& beforState, const D3D12_RESOURCE_STATES& afterState);

public:

	ID3D12Resource* GetSwapChainRenderResource(const UINT& indexNum) { return swapChainRenderResource_[indexNum].Get(); }

	ID3D12Resource* GetOffScreenRenderResource() { return offScreenRenderResource_.Get(); }

	D3D12_CPU_DESCRIPTOR_HANDLE& GetRtvHandles(const UINT& indexNum) { return rtvHandles_[indexNum]; }

	D3D12_CPU_DESCRIPTOR_HANDLE& GetOffScreenHandle() { return offScreenHandle_; }

private:

	// resource
	// フロントバッファとバックバッファ
	ComPtr<ID3D12Resource> swapChainRenderResource_[2];
	// 画像として書き込むバッファ
	ComPtr<ID3D12Resource> offScreenRenderResource_;

	// rtv
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];
	D3D12_CPU_DESCRIPTOR_HANDLE offScreenHandle_;

	ID3D12Device* device_ = nullptr;
	// heap
	DescriptorHeap* descriptorHeap_ = nullptr;
	// size
	DescriptorSize* descriptorSize_ = nullptr;
	// swapChain
	IDXGISwapChain4* swapChain_ = nullptr;
};