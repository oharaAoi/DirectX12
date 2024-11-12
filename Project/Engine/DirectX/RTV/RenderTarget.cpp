#include "RenderTarget.h"

RenderTarget::RenderTarget() {
}

RenderTarget::~RenderTarget() {
	Finalize();
}

void RenderTarget::Finalize() {
	for (uint32_t oi = 0; oi < renderTargetNum_; ++oi) {
		renderTargetResource_[oi].Reset();
	}
	
	swapChainRenderResource_[0].Reset();
	swapChainRenderResource_[1].Reset();
}

void RenderTarget::Init(ID3D12Device* device, DescriptorHeap* descriptorHeap, IDXGISwapChain4* swapChain) {
	assert(descriptorHeap);
	assert(swapChain);
	assert(device);

	dxHeap_ = descriptorHeap;
	swapChain_ = swapChain;
	device_ = device;

	CrateSwapChainResource();
	CreateRenderTarget();

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　RenderTargetを設定する
//////////////////////////////////////////////////////////////////////////////////////////////////
void RenderTarget::SetRenderTarget(ID3D12GraphicsCommandList* commandList, const RenderTargetType& type) {
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dxHeap_->GetDSVHeap()->GetCPUDescriptorHandleForHeapStart();
	
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles{};
	//rtvHandles = RTVHandle_[type].handleCPU;
	commandList->OMSetRenderTargets(1, &RTVHandle_[type].handleCPU, false, &dsvHandle);
	float clearColor[] = { 0.1f, 0.25f, 0.5f, 0.0f };
	// RenderTargetはoffScreen用のRenderTargetを指定しておく
	commandList->ClearRenderTargetView(RTVHandle_[type].handleCPU, clearColor, 0, nullptr);
	// 指定した深度で画面をクリア
	commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	// srv
	ID3D12DescriptorHeap* descriptorHeaps[] = { dxHeap_->GetSRVHeap() };
	commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　swawChainで使用するRenderTargetを作成する
//////////////////////////////////////////////////////////////////////////////////////////////////

void RenderTarget::CrateSwapChainResource() {
	HRESULT hr = S_FALSE;
	// バックバッファとフロントバッファを作成する
	hr = swapChain_->GetBuffer(0, IID_PPV_ARGS(&swapChainRenderResource_[0]));
	assert(SUCCEEDED(hr));
	hr = swapChain_->GetBuffer(1, IID_PPV_ARGS(&swapChainRenderResource_[1]));
	assert(SUCCEEDED(hr));

	// -------------------------------------------------
	// ↓ Resourceの作成
	// -------------------------------------------------
	D3D12_RESOURCE_DESC desc{};
	desc.Width = kWindowWidth_;			// 画面の横幅
	desc.Height = kWindowHeight_;			// 画面の縦幅
	desc.MipLevels = 1;			// 
	desc.DepthOrArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	// HEAPの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	// リソースの作成
	for (uint32_t oi = 0; oi < renderTargetNum_; ++oi) {
		hr = device_->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_ALLOW_DISPLAY,
			&desc,
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			nullptr,
			IID_PPV_ARGS(&renderTargetResource_[oi])
		);
		assert(SUCCEEDED(hr));
	}

	// -------------------------------------------------
	// ↓ Viewの作成をする
	// -------------------------------------------------
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	// 一つ目のDescriptorの生成
	swapChainRTVHandles_[0] = dxHeap_->GetDescriptorHandle(TYPE_RTV);
	device_->CreateRenderTargetView(swapChainRenderResource_[0].Get(), &rtvDesc, swapChainRTVHandles_[0].handleCPU);
	// 二つ目の生成
	swapChainRTVHandles_[1] = dxHeap_->GetDescriptorHandle(TYPE_RTV);

	device_->CreateRenderTargetView(swapChainRenderResource_[1].Get(), &rtvDesc, swapChainRTVHandles_[1].handleCPU);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　swawChain以外のRenderTargetを作成する
//////////////////////////////////////////////////////////////////////////////////////////////////

void RenderTarget::CreateRenderTarget() {
	// -------------------------------------------------
	// ↓ Resourceの作成
	// -------------------------------------------------
	D3D12_RESOURCE_DESC desc{};
	desc.Width = kWindowWidth_;			// 画面の横幅
	desc.Height = kWindowHeight_;			// 画面の縦幅
	desc.MipLevels = 1;			// 
	desc.DepthOrArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	// HEAPの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	// リソースの作成
	for (uint32_t oi = 0; oi < renderTargetNum_; ++oi) {
		HRESULT hr = device_->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_ALLOW_DISPLAY,
			&desc,
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			nullptr,
			IID_PPV_ARGS(&renderTargetResource_[oi])
		);
		assert(SUCCEEDED(hr));
	}

	// -------------------------------------------------
	// ↓ RTVを作成する
	// -------------------------------------------------
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	for (uint32_t oi = 0; oi < renderTargetNum_; ++oi) {
		RTVHandle_[oi] = dxHeap_->GetDescriptorHandle(TYPE_RTV);
		device_->CreateRenderTargetView(renderTargetResource_[oi].Get(), &rtvDesc, RTVHandle_[oi].handleCPU);
	}

	// -------------------------------------------------
	// ↓ SRVを作成する
	// -------------------------------------------------
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	for (uint32_t oi = 0; oi < renderTargetNum_; ++oi) {
		SRVHandle_[oi] = dxHeap_->AllocateSRV();
		device_->CreateShaderResourceView(renderTargetResource_[oi].Get(), &srvDesc, SRVHandle_[oi].handleCPU);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　RenderTargetの状態を遷移させる
//////////////////////////////////////////////////////////////////////////////////////////////////

void RenderTarget::TransitionResource(ID3D12GraphicsCommandList* commandList, const RenderTargetType& renderType, const D3D12_RESOURCE_STATES& beforState, const D3D12_RESOURCE_STATES& afterState) {
	TransitionResourceState(commandList, renderTargetResource_[renderType].Get(), beforState, afterState);
}

