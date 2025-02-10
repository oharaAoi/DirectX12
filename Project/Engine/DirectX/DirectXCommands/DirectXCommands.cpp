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
	CloseHandle(fenceEvent_);
	fence_.Reset();

	commandQueue_.Reset();
	for (auto oi = 0; oi < kFrameCount_; ++oi) {
		commandAllocators_[oi].Reset();
	}
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
	for (auto oi = 0; oi < kFrameCount_; ++oi) {
		hr = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocators_[oi]));
		assert(SUCCEEDED(hr));
	}
	
	// コマンドリストを生成する ----------------------------
	hr = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocators_[fenceIndex_].Get(), nullptr, IID_PPV_ARGS(&commandList_));
	assert(SUCCEEDED(hr));
}

/// <summary>
/// Fenceを作成する
/// </summary>
void DirectXCommands::CreateFence() {
	for (auto oi = 0; oi < kFrameCount_; ++oi) {
		fanceCounter_[oi] = 0;
	}

	// graphics用のフェンスの初期化 ======================================================================
	HRESULT hr = S_FALSE;
	hr = device_->CreateFence(fanceCounter_[fenceIndex_], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	assert(SUCCEEDED(hr));

	fanceCounter_[fenceIndex_]++;

	// Fenceのsignalを待つためのイベントを作成する
	fenceEvent_ = CreateEvent(NULL, false, false, NULL);
	assert(fenceEvent_ != nullptr);
}

/// <summary>
/// CPUとGPUの同期をはかる
/// </summary>
void DirectXCommands::SyncGPUAndCPU(IDXGISwapChain4* swapChain){
	// 画面の交換を行う
	swapChain->Present(1, 0);

	// 今フレームで描画コマンドを積ん方の画面のvalueを取得し、valueまでの処理が完了しら次の処理を開始する
	const auto currentValue = fanceCounter_[fenceIndex_];
	commandQueue_->Signal(fence_.Get(), currentValue);

	// 現在のbackBufferのIndexを取得する
	fenceIndex_ = swapChain->GetCurrentBackBufferIndex();
	
	if (fence_->GetCompletedValue() < fanceCounter_[fenceIndex_]) {
		// 指定下Signal(currentValueのシグナル)にたどりついていないので、たどりつくまで待つようにイベントを設定する
		fence_->SetEventOnCompletion(fanceCounter_[fenceIndex_], fenceEvent_);
		WaitForSingleObject(fenceEvent_, INFINITE);
	}

	// 次frameのfaceCounterを増やす
	fanceCounter_[fenceIndex_] = currentValue + 1;
}
