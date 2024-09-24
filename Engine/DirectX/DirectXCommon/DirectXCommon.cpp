#include "DirectXCommon.h"
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxcompiler.lib")

DirectXCommon::~DirectXCommon() {
	
}

DirectXCommon* DirectXCommon::GetInstacne() {
	static DirectXCommon instance;
	return &instance;
}

//
void DirectXCommon::Initialize(WinApp* win, int32_t backBufferWidth, int32_t backBufferHeight) {
	assert(win);
	winApp_ = win;
	kClientWidth_ = backBufferWidth;
	kClientHeight_ = backBufferHeight;

	// --------------------- DirectXの初期化 --------------------- //

#ifdef _DEBUG
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController_)))) {
		// デバックレイヤーを有効化する
		debugController_->EnableDebugLayer();
		// さらにGPU側でもチェック
		debugController_->SetEnableGPUBasedValidation(TRUE);
	}
#endif

	CreateDXGI();
	SetUseGPU();
}

void DirectXCommon::Finalize() {
	depthStencilResource_.Reset();
	CloseHandle(fenceEvent_);
	fence_.Reset();
	swapChain_.Reset();
	dxgiFactory_.Reset();
	useAdapter_.Reset();
	debugController_.Reset();
}

//------------------------------------------------------------------------------------------------------
// 命令
//------------------------------------------------------------------------------------------------------
void DirectXCommon::Begin() {
	UINT backBufferIndex = swapChain_->GetCurrentBackBufferIndex();
	ID3D12GraphicsCommandList* commandList = dxCommands_->GetCommandList();

	// ---------------------------------------------------------------
	// ↓barrierを張る(swapchainの状態がRenderTargetになっていないため)
	// ---------------------------------------------------------------
	// Transitionzで張る
	barrier_.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier_.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	// barrierを張る対象のリソース
	barrier_.Transition.pResource = renderTarget_->GetSwapChainRenderResource(backBufferIndex);
	// 遷移前のリソース
	barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	// 遷移後のResourceState
	barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	// 張る
	commandList->ResourceBarrier(1, &barrier_);

	// ---------------------------------------------------------------
	// dsv
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = descriptorHeaps_->GetDSVHeap()->GetCPUDescriptorHandleForHeapStart();
	//commandList->OMSetRenderTargets(1, &renderTarget_->GetRtvHandles(backBufferIndex), false, &dsvHandle);
	//commandList->OMSetRenderTargets(2, &renderTarget_->GetRTVHandles(), false, &dsvHandle);
	
	renderTarget_->OMSetRenderTarget(commandList, RenderTargetType::OffScreen_RenderTarget, dsvHandle);

	// 指定した深度で画面をクリア
	commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	float clearColor[] = { 0.1f, 0.25f, 0.5f, 1.0f };
	// RenderTargetはoffScreen用のRenderTargetを指定しておく
	commandList->ClearRenderTargetView(renderTarget_->GetOffScreenHandle(RenderTargetType::OffScreen_RenderTarget).handleCPU, clearColor, 0, nullptr);

	// srv
	ID3D12DescriptorHeap* descriptorHeaps[] = { descriptorHeaps_->GetSRVHeap() };
	commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	// ---------------------------------------------------------------
	commandList->RSSetViewports(1, &viewport_);
	commandList->RSSetScissorRects(1, &scissorRect_);
}

/// <summary>
/// オフスクリーン用のリソースをRenderTargetからShaderResourceにする
/// </summary>
void DirectXCommon::ChangeOffScreenResource() {
	renderTarget_->ChangeOffScreenResource(dxCommands_->GetCommandList(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

/// <summary>
/// RenderTargetをバックバッファに変更
/// </summary>
void DirectXCommon::SetSwapChain() {
	UINT backBufferIndex = swapChain_->GetCurrentBackBufferIndex();
	ID3D12GraphicsCommandList* commandList = dxCommands_->GetCommandList();
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = descriptorHeaps_->GetDSVHeap()->GetCPUDescriptorHandleForHeapStart();
	commandList->OMSetRenderTargets(1, &renderTarget_->GetRtvHandles(backBufferIndex).handleCPU, false, &dsvHandle);
}

void DirectXCommon::End() {
	// ---------------------------------------------------------------
	// ↓RTVの画面から画面表示できるようにする
	// ---------------------------------------------------------------
	barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

	// 張る
	dxCommands_->GetCommandList()->ResourceBarrier(1, &barrier_);

	// ---------------------------------------------------------------
	HRESULT hr = dxCommands_->GetCommandList()->Close();
	assert(SUCCEEDED(hr));

	ID3D12CommandList* commandLists[] = { dxCommands_->GetCommandList() };
	dxCommands_->GetCommandQueue()->ExecuteCommandLists(1, commandLists);

	swapChain_->Present(1, 0);

	// ---------------------------------------------------------------
	// CPUとGPUの同期をとる
	dxCommands_->SyncGPUAndCPU();

	// ---------------------------------------------------------------
	hr = dxCommands_->GetCommandAllocator()->Reset();
	assert(SUCCEEDED(hr));
	hr = dxCommands_->GetCommandList()->Reset(dxCommands_->GetCommandAllocator(), nullptr);
	assert(SUCCEEDED(hr));

	// offScreenの設定を直す
	renderTarget_->ChangeOffScreenResource(dxCommands_->GetCommandList(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
}

void DirectXCommon::CreateDXGI() {
	dxgiFactory_ = nullptr;
	// HRESULはwidows系のエラーコードであり
	// 関数が成功したかどうかをSUCCEEDEマクロ判定で判定できる
	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));

	// 初期化の根本的な部分でエラーが出た場合はプログラムが間違っているか、
	// どうにもできない場合が多いのでassertにしておく
	assert(SUCCEEDED(hr));
}

void DirectXCommon::SetUseGPU() {
	HRESULT hr;
	// 使用するアダプタ用の変数。最初にnullptrを入れておく
	useAdapter_ = nullptr;
	// 良い順にアダプタを頼む
	for (UINT i = 0; dxgiFactory_->EnumAdapterByGpuPreference(i,
															  DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter_)) != DXGI_ERROR_NOT_FOUND; ++i) {

		// アダプターの情報を取得する
		DXGI_ADAPTER_DESC3 adapterDesc{};
		hr = useAdapter_->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr)); // 取得出来なかった時用
		// ソフトウェアのアダプタでなければ採用
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			// 採用したアダプタの情報の情報をログに出力.wstrinfgの方
			Log(ConvertString(std::format(L"Use Adapater:{}\n", adapterDesc.Description)));
			break;
		}

		useAdapter_ = nullptr; // ソフトウェアアダプタの場合は見なかったことにする
	}

	// 適切なアダプタが見つからなかったら軌道できない
	assert(useAdapter_ != nullptr);
}

void DirectXCommon::SetError() {
#ifdef _DEBUG

	ComPtr<ID3D12InfoQueue> infoQueue = nullptr;
	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		// ヤバいエラージに止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		// エラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		// 警告時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

		// 抑制するメッセージのID ------------------------------
		D3D12_MESSAGE_ID denyIds[] = {
			// Windows11でのDXGIデバックレイヤーとDX12デバックレイヤーの相互作用にバグによるメッセージ
			// https://stackoverflow.com.questions/69805245/directx-12-application-is-crashing-in-windows-11
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};
		// 抑制するレベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		//指定してメッセージの表示を抑制する
		infoQueue->PushStorageFilter(&filter);
	}

#endif // DEBUG
}

//------------------------------------------------------------------------------------------------------
// 設定
//------------------------------------------------------------------------------------------------------
void DirectXCommon::Setting(ID3D12Device* device, DirectXCommands* dxCommands, DescriptorHeap* descriptorHeaps, RenderTarget* renderTarget) {
	assert(device);
	assert(descriptorHeaps);
	assert(dxCommands);
	assert(renderTarget);

	device_ = device;
	descriptorHeaps_ = descriptorHeaps;
	dxCommands_ = dxCommands;
	renderTarget_ = renderTarget;

	// ----------------------------------------------------------------------------------------
	SetError();

	// ----------------------------------------------------------------------------------------
	// swapchain
	CreateSwapChain();
	// fence
	CrateFence();
	// DSV
	CreateDSV();
	// viewportとscissor
	SetViewport();
}

//------------------------------------------------------------------------------------------------------
// swapChainの生成
//------------------------------------------------------------------------------------------------------
void DirectXCommon::CreateSwapChain() {
	HRESULT hr = S_FALSE;
	DXGI_SWAP_CHAIN_DESC1 desc{};
	desc.Width = kClientWidth_;							// 画面の幅
	desc.Height = kClientHeight_;						// 画面の高さ
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;			// 色の形式
	desc.SampleDesc.Count = 1;							// マルチサンプルしない
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 描画のターゲットとして利用する
	desc.BufferCount = 2;								// ダブルバッファ
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	// モニタに移したら中身を破棄

	// bufferCountを保存しておく
	swapChainBufferCount_ = desc.BufferCount;

	// コマンドキュー、ウィンドウハンドルを設定して生成する
	hr = dxgiFactory_->CreateSwapChainForHwnd(dxCommands_->GetCommandQueue(), winApp_->GetHwnd(), &desc, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(swapChain_.GetAddressOf()));
	assert(SUCCEEDED(hr));
}

//------------------------------------------------------------------------------------------------------
// Fenceの生成
//------------------------------------------------------------------------------------------------------
void DirectXCommon::CrateFence() {
	HRESULT hr = S_FALSE;
	fenceValue_ = 0;
	hr = device_->CreateFence(fenceValue_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	assert(SUCCEEDED(hr));

	// Fenceのsignalを待つためのイベントを作成する
	fenceEvent_ = CreateEvent(NULL, false, false, NULL);
	assert(fenceEvent_ != nullptr);
}

//------------------------------------------------------------------------------------------------------
// ↓viewportの生成
//------------------------------------------------------------------------------------------------------
void DirectXCommon::SetViewport() {
	// ビューポート
	// クライアント領域のサイズと一緒にして画面全体を表示
	viewport_.Width = static_cast<float>(kClientWidth_);
	viewport_.Height = static_cast<float>(kClientHeight_);
	viewport_.TopLeftX = 0;
	viewport_.TopLeftY = 0;
	viewport_.MinDepth = 0.0f;
	viewport_.MaxDepth = 1.0f;

	// シザー矩形
	// 基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect_.left = 0;
	scissorRect_.right = static_cast<LONG>(kClientWidth_);
	scissorRect_.top = 0;
	scissorRect_.bottom = static_cast<LONG>(kClientHeight_);
}

void DirectXCommon::CreateDSV() {
	depthStencilResource_ = CreateDepthStencilTextureResource(device_, kClientWidth_, kClientHeight_);

	// heap上にDSCを構築
	D3D12_DEPTH_STENCIL_VIEW_DESC desc{};
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	device_->CreateDepthStencilView(depthStencilResource_.Get(), &desc, descriptorHeaps_->GetDescriptorHandle(DescriptorHeapType::TYPE_DSV).handleCPU);
}
