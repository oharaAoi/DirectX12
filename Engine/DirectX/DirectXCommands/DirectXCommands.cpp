#include "DirectXCommands.h"

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
	CloseHandle(computeFenceEvent_);
	computeFence_.Reset();
	CloseHandle(fenceEvent_);
	fence_.Reset();

	computeCommandQueue_.Reset();
	computeCommandAllocator_.Reset();
	computeCommandList_.Reset();

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

	// computeShader用のコマンド系の初期化 ======================================================================
	// コマンドキューを生成する
	D3D12_COMMAND_QUEUE_DESC computeQueueDesc = {};
	computeQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
	computeQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	hr = device_->CreateCommandQueue(&computeQueueDesc, IID_PPV_ARGS(&computeCommandQueue_));
	assert(SUCCEEDED(hr));

	// コマンドアロケータを生成する
	hr = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COMPUTE, IID_PPV_ARGS(&computeCommandAllocator_));
	assert(SUCCEEDED(hr));

	// コマンドリストを生成する ----------------------------
	hr = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_COMPUTE, computeCommandAllocator_.Get(), nullptr, IID_PPV_ARGS(&computeCommandList_));
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

	// Fenceのsignalを待つためのイベントを作成する
	fenceEvent_ = CreateEvent(NULL, false, false, NULL);
	assert(fenceEvent_ != nullptr);

	// computeShader用のフェンスの初期化 ======================================================================
	computeFenceValue_ = 0;
	hr = device_->CreateFence(computeFenceValue_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&computeFence_));
	assert(SUCCEEDED(hr));

	// Fenceのsignalを待つためのイベントを作成する
	computeFenceEvent_ = CreateEvent(NULL, false, false, NULL);
	assert(computeFenceEvent_ != nullptr);

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
	if (fence_->GetCompletedValue() < fenceValue_) {
		// 指定下Signalにたどりついていないので、たどりつくまで松ようにイベントを設定する
		fence_->SetEventOnCompletion(fenceValue_, fenceEvent_);

		WaitForSingleObject(fenceEvent_, INFINITE);
	}
}

/// <summary>
/// コンピュートシェーダーを行った後にGPUとCPU
/// </summary>
void DirectXCommands::ComputeShaderSyncGPUAndCPU() {
	// fenceの値を更新
	computeFenceValue_++;
	// GPUがここまでたどり着いた時に,fenceの値を指定した値に第謬するようにsignelを送る
	computeCommandQueue_->Signal(fence_.Get(), computeFenceValue_);

	// Fenceの値が指定したSignal値にたどりついているか確認する
	// GetCompletedValueの初期値はFence作成時に渡した初期値
	if (fence_->GetCompletedValue() < computeFenceValue_) {
		// 指定下Signalにたどりついていないので、たどりつくまで松ようにイベントを設定する
		fence_->SetEventOnCompletion(computeFenceValue_, computeFenceEvent_);

		WaitForSingleObject(computeFenceEvent_, INFINITE);
	}
}
