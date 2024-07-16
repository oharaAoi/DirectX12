#include "RenderTarget.h"

RenderTarget::RenderTarget() {
}

RenderTarget::~RenderTarget() {
	Finalize();
}

void RenderTarget::Finalize() {
	offScreenRenderResource_.Reset();
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

	// Resourceの作成
	CreateRenderResource();
	// オフスクリーン用の作成
	CreateOffScreenResource();

	// viewの作成
	CreateRenderTargetView();
	CreateOffScreenView();
}

/// <summary>
/// バックバッファとフロントバッファのResource作成
/// </summary>
void RenderTarget::CreateRenderResource() {
	HRESULT hr = S_FALSE;
	// バックバッファとフロントバッファを作成する
	hr = swapChain_->GetBuffer(0, IID_PPV_ARGS(&swapChainRenderResource_[0]));
	assert(SUCCEEDED(hr));
	hr = swapChain_->GetBuffer(1, IID_PPV_ARGS(&swapChainRenderResource_[1]));
	assert(SUCCEEDED(hr));
}

/// <summary>
/// バックバッファとフロントバッファのViewの作成
/// </summary>
void RenderTarget::CreateRenderTargetView() {
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	// 先頭を取得
	//D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle = GetCPUDescriptorHandle(dxHeap_->GetRTVHeap(), dxHeap_->GetDescriptorSize()->GetRTV(), 0);
	//// 一つ目のDescriptorの生成
	//rtvHandles_[0] = rtvStartHandle;
	//device_->CreateRenderTargetView(swapChainRenderResource_[0].Get(), &rtvDesc, rtvHandles_[0]);
	//// 二つ目の生成
	//rtvHandles_[1].ptr = rtvHandles_[0].ptr + device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// 一つ目のDescriptorの生成
	rtvHandles_[0] = dxHeap_->GetDescriptorHandle(DescriptorHeapType::TYPE_RTV);
	device_->CreateRenderTargetView(swapChainRenderResource_[0].Get(), &rtvDesc, rtvHandles_[0].handleCPU);
	// 二つ目の生成
	rtvHandles_[1] = dxHeap_->GetDescriptorHandle(DescriptorHeapType::TYPE_RTV);

	device_->CreateRenderTargetView(swapChainRenderResource_[1].Get(), &rtvDesc, rtvHandles_[1].handleCPU);
}

/// <summary>
/// オフスクリーン用のResource作成
/// </summary>
void RenderTarget::CreateOffScreenResource() {
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
	HRESULT hr = device_->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_ALLOW_DISPLAY,
		&desc,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		nullptr,
		IID_PPV_ARGS(&offScreenRenderResource_)
	);

	assert(SUCCEEDED(hr));
}

/// <summary>
/// オフスクリーン用のViewの作成
/// </summary>
void RenderTarget::CreateOffScreenView() {
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	offScreenHandle_ = dxHeap_->GetDescriptorHandle(DescriptorHeapType::TYPE_RTV);
	
	//offScreenHandle_.handleCPU.ptr = rtvHandles_[1].handleCPU.ptr + device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	device_->CreateRenderTargetView(offScreenRenderResource_.Get(), &rtvDesc, offScreenHandle_.handleCPU);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	offScreenSRVHandle_ = dxHeap_->GetDescriptorHandle(DescriptorHeapType::TYPE_SRV);

	device_->CreateShaderResourceView(offScreenRenderResource_.Get(), &srvDesc, offScreenSRVHandle_.handleCPU);
}

/// <summary>
/// オフスクリーン用のリソースのステートを変更する
/// </summary>
void RenderTarget::ChangeOffScreenResource(ID3D12GraphicsCommandList* commandList, const D3D12_RESOURCE_STATES& beforState, const D3D12_RESOURCE_STATES& afterState) {
	TransitionResourceState(commandList, offScreenRenderResource_.Get(), beforState, afterState);
}
