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

static const uint32_t renderTargetNum_ = 3;

enum RenderTargetType {
	OffScreen_RenderTarget,
	DepthOfField_RenderTarget,
	EffectSystem_RenderTarget
};

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

	void ChangeRTVResource(ID3D12GraphicsCommandList* commandList, const RenderTargetType& renderType,  const D3D12_RESOURCE_STATES& beforState, const D3D12_RESOURCE_STATES& afterState);

	/// <summary>
	/// レンダーターゲットを設定する関数
	/// </summary>
	/// <param name="commandList"></param>
	/// <param name="dsvHandle"></param>
	void OMSetRenderTarget(ID3D12GraphicsCommandList* commandList, const RenderTargetType& renderType, const D3D12_CPU_DESCRIPTOR_HANDLE& dsvHandle);

public:

	ID3D12Resource* GetSwapChainRenderResource(const UINT& indexNum) { return swapChainRenderResource_[indexNum].Get(); }

	ID3D12Resource* GetOffScreenRenderResource(const RenderTargetType& type) const {
		return renderTargetResource_[type].Get();
	}

	DescriptorHeap::DescriptorHandles& GetRtvHandles(const UINT& indexNum) { return swapChainRTVHandles_[indexNum]; }

	const DescriptorHeap::DescriptorHandles& GetOffScreenHandle(const RenderTargetType& type) const {
		return RTVHandle_[type];
	}

	const DescriptorHeap::DescriptorHandles& GetOffScreenSRVHandle(const RenderTargetType& type) const {
		return SRVHandle_[type];
	}

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