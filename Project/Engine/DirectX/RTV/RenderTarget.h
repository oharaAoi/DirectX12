#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <wrl.h>
// utilities
#include "Engine/Utilities/Convert.h"
#include "Engine/Utilities/DirectXUtils.h"
// heap
#include "Engine/DirectX/Descriptor/DescriptorHeap.h"
#include "Engine/DirectX/Descriptor/DescriptorSize.h"

template<typename T>
using ComPtr = Microsoft::WRL::ComPtr <T>;

enum RenderTargetType {
	Object3D_RenderTarget,
	Sprite2D_RenderTarget,
	OffScreen_RenderTarget,
	EffectSystem_RenderTarget,
	DepthOfField_RenderTarget,
};

static const uint32_t renderTargetNum_ = sizeof(RenderTargetType);

class RenderTarget {
public:

	RenderTarget();
	~RenderTarget();

	void Finalize();

	void Init(ID3D12Device* device, DescriptorHeap* descriptorHeap, IDXGISwapChain4* swapChain);

	void SetRenderTarget(ID3D12GraphicsCommandList* commandList, const RenderTargetType& type);

	/// <summary>
	/// swawChainで使用するRenderTargetを作成する
	/// </summary>
	void CrateSwapChainResource();

	/// <summary>
	/// swawChain以外のRenderTargetを作成する
	/// </summary>
	void CreateRenderTarget();

	void TransitionResource(ID3D12GraphicsCommandList* commandList, const RenderTargetType& renderType, const D3D12_RESOURCE_STATES& beforState, const D3D12_RESOURCE_STATES& afterState);

public:

	ID3D12Resource* GetSwapChainRenderResource(const UINT& indexNum) { return swapChainRenderResource_[indexNum].Get(); }

	DescriptorHeap::DescriptorHandles& GetRtvHandles(const UINT& indexNum) { return swapChainRTVHandles_[indexNum]; }

	const DescriptorHeap::DescriptorHandles& GetOffScreenHandle(const RenderTargetType& type) const { return RTVHandle_[type]; }
	const DescriptorHeap::DescriptorHandles& GetOffScreenSRVHandle(const RenderTargetType& type) const { return SRVHandle_[type]; }

private:

	// resource
	// フロントバッファとバックバッファ
	ComPtr<ID3D12Resource> swapChainRenderResource_[2];
	// 画像として書き込むバッファ
	ComPtr<ID3D12Resource> renderTargetResource_[renderTargetNum_];

	// rtv
	DescriptorHeap::DescriptorHandles swapChainRTVHandles_[2];

	DescriptorHeap::DescriptorHandles RTVHandle_[renderTargetNum_];
	// srv
	DescriptorHeap::DescriptorHandles SRVHandle_[renderTargetNum_];

	ID3D12Device* device_ = nullptr;
	// heap
	DescriptorHeap* dxHeap_ = nullptr;
	// swapChain
	IDXGISwapChain4* swapChain_ = nullptr;
};