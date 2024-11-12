#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <wrl.h>
#include <memory>
// winapp
#include "Engine/WinApp/WinApp.h"
// DirectX
#include "Engine/DirectX/Descriptor/DescriptorHeap.h"
#include "Engine/DirectX/DirectXCommands/DirectXCommands.h"
// DXC
#include <dxcapi.h>
// RT
#include "Engine/DirectX/RTV/RenderTarget.h"
// utilities
#include "Engine/Utilities/Convert.h"
#include "Engine/Utilities/DirectXUtils.h"
#include "Engine/Utilities/Debug.h"

template<typename T>
using ComPtr = Microsoft::WRL::ComPtr <T>;

class DirectXCommon {
public:

	DirectXCommon() = default;
	~DirectXCommon();
	DirectXCommon(const DirectXCommon&) = delete;
	const DirectXCommon& operator=(const DirectXCommon&) = delete;

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static DirectXCommon* GetInstacne();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* win, int32_t backBufferWidth, int32_t backBufferHeight);

	/// <summary>
	/// 色々な設定をする
	/// </summary>
	void Setting(ID3D12Device* device, DirectXCommands* dxCommands, DescriptorHeap* descriptorHeaps, RenderTarget* renderTarget);

	/// <summary>
	/// 終了関数
	/// </summary>
	void Finalize();

	/// <summary>
	/// フレームを開始する
	/// </summary>
	void Begin();

	/// <summary>
	/// フレームを終了する
	/// </summary>
	void End();

	void SetSwapChain();

public:

	/// <summary>
	/// DXGIFactoryの生成
	/// </summary>
	void CreateDXGI();

	/// <summary>
	/// 使用するGUPを決める
	/// </summary>
	void SetUseGPU();

	void SetError();

	/// <summary>
	/// swapchainを生成する
	/// </summary>
	/// <param name="commandQueue"></param>
	void CreateSwapChain();

	/// <summary>
	/// Fenceの生成
	/// </summary>
	void CrateFence();

	/// <summary>
	/// Viewport変換
	/// </summary>
	void SetViewport();

	/// <summary>
	/// DSVの設定をする
	/// </summary>
	void CreateDSV();

////////////////////////////////////////////////////////////////////////////////////
// accessor
////////////////////////////////////////////////////////////////////////////////////

	int32_t GetSwapChainBfCount() {return swapChainBufferCount_;}

public:

	IDXGIAdapter4* GetUseAdapter() { return useAdapter_.Get(); }

	ComPtr<IDXGISwapChain4> GetSwapChain() { return swapChain_.Get(); }

	ID3D12Resource* GetDepthStencilResource() { return depthStencilResource_.Get(); }

private:

	int32_t kClientWidth_;
	int32_t kClientHeight_;

	int32_t swapChainBufferCount_;

	WinApp* winApp_ = nullptr;
	DescriptorHeap* descriptorHeaps_ = nullptr;
	DirectXCommands* dxCommands_ = nullptr;
	ID3D12Device* device_ = nullptr;

	RenderTarget* renderTarget_ = nullptr;

private:

	// デバック =======================================================================================
	ComPtr<ID3D12Debug1> debugController_ = nullptr;

	// 初期化系 =======================================================================================
	ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;
	ComPtr<IDXGIAdapter4> useAdapter_ = nullptr;

	// 生成する変数 =======================================================================================
	// swapChaim
	ComPtr<IDXGISwapChain4> swapChain_ = nullptr;
	
	// Fence & Event
	ComPtr<ID3D12Fence> fence_ = nullptr;
	uint64_t fenceValue_;
	HANDLE fenceEvent_;

	// barrier
	D3D12_RESOURCE_BARRIER barrier_;

	D3D12_VIEWPORT viewport_;
	D3D12_RECT scissorRect_;

	// dsv
	ComPtr<ID3D12Resource> depthStencilResource_ = nullptr;
};

