#include "DirectXCommands.h"
#include <synchapi.h>

DirectXCommands::DirectXCommands(ID3D12Device* device) {
	Initialize(device);
}

DirectXCommands::~DirectXCommands() {
}

void DirectXCommands::Initialize(ID3D12Device* device) {
	assert(device);
	device_ = device;

	CreateCommand();
	CreateFence();
}

void DirectXCommands::Finalize() {
	fence_.Reset();

	commandQueue_.Reset();
	commandAllocator_.Reset();
	commandList_.Reset();
}

/// <summary>
/// DirectX12のコマンドを生成する
/// </summary>
void DirectXCommands::CreateCommand() {
	HRESULT hr = S_FALSE;
	// graphics用のコマンド系の初期化 ======================================================================
	// GPUに命令を投げてくれる人　--------------------------
	// コマンドキューを生成する
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	hr = device_->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue_));
	assert(SUCCEEDED(hr));

	// CommandAllocatorの生成 --------------------------------
	// コマンドアロケータを生成する
	hr = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
	assert(SUCCEEDED(hr));

	// コマンドリストを生成する ----------------------------
	hr = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));
	assert(SUCCEEDED(hr));
}

/// <summary>
/// Fenceを作成する
/// </summary>
void DirectXCommands::CreateFence() {
	// graphics用のフェンスの初期化 ======================================================================
	HRESULT hr = S_FALSE;
	fenceValue_ = 0;
	hr = device_->CreateFence(fenceValue_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	assert(SUCCEEDED(hr));
}

/// <summary>
/// CPUとGPUの同期をはかる
/// </summary>
void DirectXCommands::SyncGPUAndCPU() {
	// fenceの値を更新
	fenceValue_++;
	// GPUがここまでたどり着いた時に,fenceの値を指定した値に第謬するようにsignelを送る
	commandQueue_->Signal(fence_.Get(), fenceValue_);

	// Fenceの値が指定したSignal値にたどりついているか確認する
	// GetCompletedValueの初期値はFence作成時に渡した初期値
	if (fence_->GetCompletedValue() != fenceValue_) {
		// 指定下Signalにたどりついていないので、たどりつくまで松ようにイベントを設定する
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence_->SetEventOnCompletion(fenceValue_, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
		
	}

	gameTimer_.WaitNextFrame();
}
