#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <wrl.h>
// utilities
#include "Engine/Utilities/Convert.h"
#include "Engine/Utilities/DirectXUtils.h"
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
	void SyncGPUAndCPU();

	/// <summary>
	/// コンピュートシェーダーを行った後にGPUとCPU
	/// </summary>
	void EffectShaderSyncGPUAndCPU();

public:
	/// <summary>
	/// commandListをGPUに投げて実行させるもの
	/// </summary>
	/// <returns></returns>
	ID3D12CommandQueue* GetCommandQueue() { return commandQueue_.Get(); }

	ID3D12CommandAllocator* GetCommandAllocator() { return commandAllocator_.Get(); }

	ID3D12GraphicsCommandList* GetCommandList() { return commandList_.Get(); }

private:

	ID3D12Device* device_ = nullptr;
	
	// graphics用のコマンド系 ---------------------------------------------
	ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
	ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;
	ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;

	// Fence & Event
	ComPtr<ID3D12Fence> fence_ = nullptr;
	uint64_t fenceValue_;
	HANDLE fenceEvent_;

	// computeShader用のコマンド系 ---------------------------------------------
	ComPtr<ID3D12CommandQueue> effectCommandQueue_ = nullptr;
	ComPtr<ID3D12CommandAllocator> effectCommandAllocator_ = nullptr;
	ComPtr<ID3D12GraphicsCommandList> effectCommandList_ = nullptr;

	// Fence & Event
	ComPtr<ID3D12Fence> effectFence_ = nullptr;
	uint64_t effectFenceValue_;
	HANDLE effectFenceEvent_;
};