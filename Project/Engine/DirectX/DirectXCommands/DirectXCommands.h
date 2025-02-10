#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <wrl.h>
// utilities
#include "Engine/Utilities/Convert.h"
#include "Engine/DirectX/Utilities/DirectXUtils.h"
// DirectX系
#include "Engine/DirectX/DirectXDevice/DirectXDevice.h"

template<typename T>
using ComPtr = Microsoft::WRL::ComPtr <T>;

class DirectXCommands {
public:

	DirectXCommands(ID3D12Device* device);
	~DirectXCommands();

	void Initialize(ID3D12Device* device);

	void Finalize();

	/// <summary>
	/// DirectX12のコマンドを生成する
	/// </summary>
	void CreateCommand();

	/// <summary>
	/// Fenceを作成する
	/// </summary>
	void CreateFence();

	/// <summary>
	/// CPUとGPUの同期をはかる
	/// </summary>
	void SyncGPUAndCPU(IDXGISwapChain4* swapChain);


public:
	/// <summary>
	/// commandListをGPUに投げて実行させるもの
	/// </summary>
	/// <returns></returns>
	ID3D12CommandQueue* GetCommandQueue() { return commandQueue_.Get(); }

	ID3D12CommandAllocator* GetCommandAllocator() { return commandAllocators_[fenceIndex_].Get(); }

	ID3D12GraphicsCommandList* GetCommandList() { return commandList_.Get(); }

private:

	static const uint32_t kFrameCount_ = 2;

	ID3D12Device* device_ = nullptr;
	
	// graphics用のコマンド系 ---------------------------------------------
	ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
	ComPtr<ID3D12CommandAllocator> commandAllocators_[kFrameCount_];
	ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;

	// Fence & Event
	ComPtr<ID3D12Fence> fence_ = nullptr;
	HANDLE fenceEvent_;
	uint64_t fanceCounter_[kFrameCount_];
	uint32_t fenceIndex_;

};