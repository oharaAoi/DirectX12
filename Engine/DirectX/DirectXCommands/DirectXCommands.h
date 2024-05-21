#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <wrl.h>
// utilities
#include "Convert.h"
#include "DirectXUtils.h"
// DirectX系
#include "DirectXDevice.h"

template<typename T>
using Comptr = Microsoft::WRL::ComPtr <T>;

class DirectXCommands {
public:

	DirectXCommands(ID3D12Device* device);
	~DirectXCommands();

	void Initialize(ID3D12Device* device);

	void CreateCommand();

public:
	/// <summary>
	/// commandListをGPUに投げて実行させるもの
	/// </summary>
	/// <returns></returns>
	ID3D12CommandQueue* GetCommandQueue() { return commandQueue_.Get(); }

	ID3D12CommandAllocator* GetCommandAllocator() { return commandAllocator_.Get(); }

	ID3D12GraphicsCommandList* GetCommandList() { return commandList_.Get(); }

private:

	Comptr<ID3D12CommandQueue> commandQueue_ = nullptr;
	Comptr<ID3D12CommandAllocator> commandAllocator_ = nullptr;
	Comptr<ID3D12GraphicsCommandList> commandList_ = nullptr;

	ID3D12Device* device_ = nullptr;

};