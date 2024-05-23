#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <wrl.h>
#include <memory>
// winapp
#include "WinApp.h"
// DirectX
#include "DescriptorSize.h"
#include "DescriptorHeap.h"
#include "DirectXCommands.h"
// DXC
#include <dxcapi.h>
// utilities
#include "Convert.h"
#include "DirectXUtils.h"
#include "Debug.h"

template<typename T>
using Comptr = Microsoft::WRL::ComPtr <T>;

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
	void Setting(ID3D12Device* device, DirectXCommands* dxCommands, DescriptorHeap* descriptorHeaps);

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
	/// RTVを生成
	/// </summary>
	void CreateRTV();

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
// accseser
////////////////////////////////////////////////////////////////////////////////////

	int32_t GetSwapChainBfCount() {return swapChainBufferCount_;}

public:

	IDXGIAdapter4* GetUseAdapter() { return useAdapter_.Get(); }

	DescriptorSize* GetDescriptorSize() { return descriptorSize_.get(); }

private:

	int32_t kClientWidth_;
	int32_t kClientHeight_;

	int32_t swapChainBufferCount_;

	WinApp* winApp_ = nullptr;
	DescriptorHeap* descriptorHeaps_ = nullptr;
	DirectXCommands* dxCommands_ = nullptr;
	ID3D12Device* device_ = nullptr;

	std::unique_ptr<DescriptorSize> descriptorSize_;

private:

	// デバック =======================================================================================
	Comptr<ID3D12Debug1> debugController_ = nullptr;

	// 初期化系 =======================================================================================
	Comptr<IDXGIFactory7> dxgiFactory_ = nullptr;
	Comptr<IDXGIAdapter4> useAdapter_ = nullptr;

	// 生成する変数 =======================================================================================
	// swapChaim
	Comptr<IDXGISwapChain4> swapChain_ = nullptr;
	Comptr<ID3D12Resource> swapChainResource_[2];

	// RTV
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];

	// Fence & Event
	Comptr<ID3D12Fence> fence_ = nullptr;
	uint64_t fenceValue_;
	HANDLE fenceEvent_;

	// barrier
	D3D12_RESOURCE_BARRIER barrier_;

	D3D12_VIEWPORT viewport_;
	D3D12_RECT scissorRect_;

	// dsv
	Comptr<ID3D12Resource> depthStencilResource_ = nullptr;
};

