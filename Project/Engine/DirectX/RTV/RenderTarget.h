#pragma once
#include "Engine/Utilities/Convert.h"
#include "Engine/Utilities/DirectXUtils.h"
#include "Engine/DirectX/Descriptor/DescriptorHeap.h"
#include "Engine/DirectX/Descriptor/DescriptorSize.h"
#include "Engine/DirectX/Resource/DxResource.h"

template<typename T>
using ComPtr = Microsoft::WRL::ComPtr <T>;

enum RenderTargetType {
	Object3D_RenderTarget,
	OffScreen_RenderTarget,
	EffectSystem_RenderTarget,
	PreEffectSystem_RenderTarget,
};

static const uint32_t renderTargetNum_ = sizeof(RenderTargetType) + 1;

class RenderTarget {
public:

	RenderTarget();
	~RenderTarget();

	void Finalize();

	void Init(ID3D12Device* device, DescriptorHeap* descriptorHeap, IDXGISwapChain4* swapChain);

	void SetRenderTarget(ID3D12GraphicsCommandList* commandList, const RenderTargetType& type);

	void ClearDepth(ID3D12GraphicsCommandList* commandList);

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

	ID3D12Resource* GetSwapChainRenderResource(const UINT& indexNum) { return swapChainResource_[indexNum]->GetResource(); }

	const DescriptorHeap::DescriptorHandles& GetSwapChainHandle(const UINT& indexNum) { return swapChainResource_[indexNum]->GetRTV(); }

	const DescriptorHeap::DescriptorHandles& GetRenderTargetRTVHandle(const RenderTargetType& type) const { return renderTargetResource_[type]->GetRTV(); }
	const DescriptorHeap::DescriptorHandles& GetRenderTargetSRVHandle(const RenderTargetType& type) const { return renderTargetResource_[type]->GetSRV(); }

private:

	std::unique_ptr<DxResource> swapChainResource_[2];
	std::unique_ptr<DxResource> renderTargetResource_[renderTargetNum_];

	ID3D12Device* device_ = nullptr;
	// heap
	DescriptorHeap* dxHeap_ = nullptr;
	// swapChain
	IDXGISwapChain4* swapChain_ = nullptr;
};