#include "RenderTarget.h"

RenderTarget::RenderTarget() {
}

RenderTarget::~RenderTarget() {
	Finalize();
}

void RenderTarget::Finalize() {
	for (uint32_t oi = 0; oi < renderTargetNum_; ++oi) {
		renderTargetResource_[oi]->Finalize();
	}
	
	swapChainResource_[0]->Finalize();
	swapChainResource_[1]->Finalize();
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
	commandList->OMSetRenderTargets(1, &renderTargetResource_[type]->GetRTV().handleCPU, false, &dsvHandle);
	float clearColor[] = { 0.1f, 0.25f, 0.5f, 0.0f };
	// RenderTargetはoffScreen用のRenderTargetを指定しておく
	commandList->ClearRenderTargetView(renderTargetResource_[type]->GetRTV().handleCPU, clearColor, 0, nullptr);
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

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	for (uint32_t oi = 0; oi < 2; ++oi) {
		swapChainResource_[oi] = std::make_unique<DxResource>();
		swapChainResource_[oi]->Init(device_, dxHeap_, ResourceType::RENDERTARGET);

		swapChainResource_[oi]->CreateResource(&desc, &heapProperties, D3D12_HEAP_FLAG_ALLOW_DISPLAY, D3D12_RESOURCE_STATE_RENDER_TARGET);
		swapChainResource_[oi]->SetSwapChainBuffer(swapChain_, oi);
		swapChainResource_[oi]->CreateRTV(rtvDesc);
	}

	//// バックバッファとフロントバッファを作成する
	//hr = swapChain_->GetBuffer(0, IID_PPV_ARGS(&swapChainResource_[0]->GetCompResource()));
	//assert(SUCCEEDED(hr));
	//hr = swapChain_->GetBuffer(1, IID_PPV_ARGS(&swapChainResource_[1]->GetCompResource()));
	//assert(SUCCEEDED(hr));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　swawChain以外のRenderTargetを作成する
//////////////////////////////////////////////////////////////////////////////////////////////////

void RenderTarget::CreateRenderTarget() {
	// resourceの設定
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

	// RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	// SRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	// 実際の初期化
	for (uint32_t oi = 0; oi < renderTargetNum_; ++oi) {
		renderTargetResource_[oi] = std::make_unique<DxResource>();
		renderTargetResource_[oi]->Init(device_, dxHeap_, ResourceType::RENDERTARGET);

		renderTargetResource_[oi]->CreateResource(&desc, &heapProperties, D3D12_HEAP_FLAG_ALLOW_DISPLAY, D3D12_RESOURCE_STATE_RENDER_TARGET);

		renderTargetResource_[oi]->CreateRTV(rtvDesc);
		renderTargetResource_[oi]->CreateSRV(srvDesc);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　RenderTargetの状態を遷移させる
//////////////////////////////////////////////////////////////////////////////////////////////////

void RenderTarget::TransitionResource(ID3D12GraphicsCommandList* commandList, const RenderTargetType& renderType, const D3D12_RESOURCE_STATES& beforState, const D3D12_RESOURCE_STATES& afterState) {
	renderTargetResource_[renderType]->Transition(commandList, beforState, afterState);
}

