#include "DxResource.h"
#include <cassert>

DxResource::DxResource() {}
DxResource::~DxResource() {}

void DxResource::Finalize() {
	cBuffer_.Reset();
	if (srvAddress_ != std::nullopt) {
		DescriptorHeap::AddFreeSrvList(srvAddress_.value().assignIndex_);
	}
	if (uavAddress_ != std::nullopt) {
		DescriptorHeap::AddFreeSrvList(uavAddress_.value().assignIndex_);
	}
	if (rtvAddress_ != std::nullopt) {
		DescriptorHeap::AddFreeSrvList(rtvAddress_.value().assignIndex_);
	}
}

void DxResource::Init(ID3D12Device* device, DescriptorHeap* dxHeap, ResourceType type) {

	assert(device);
	assert(dxHeap);

	pDevice_ = device;
	pDxHeap_ = dxHeap;

	type_ = type;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void DxResource::CreateResource(D3D12_RESOURCE_DESC* resourceDesc, D3D12_HEAP_PROPERTIES* heapProperties,
									const D3D12_HEAP_FLAGS& heapFlags, const D3D12_RESOURCE_STATES& resourceState) {

	HRESULT hr = pDevice_->CreateCommittedResource(
		heapProperties,
		heapFlags,
		resourceDesc,
		resourceState,
		nullptr,
		IID_PPV_ARGS(&cBuffer_)
	);
	assert(SUCCEEDED(hr));

	bufferState_ = resourceState;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　SRVの生成
//////////////////////////////////////////////////////////////////////////////////////////////////

void DxResource::CreateSRV(const D3D12_SHADER_RESOURCE_VIEW_DESC& desc) {
	srvAddress_ = pDxHeap_->AllocateSRV();
	pDevice_->CreateShaderResourceView(cBuffer_.Get(), &desc, srvAddress_.value().handleCPU);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓ UAVの生成
//////////////////////////////////////////////////////////////////////////////////////////////////

void DxResource::CreateUAV(const D3D12_UNORDERED_ACCESS_VIEW_DESC& desc) {
	uavAddress_ = pDxHeap_->AllocateSRV();
	pDevice_->CreateUnorderedAccessView(cBuffer_.Get(), nullptr, &desc, uavAddress_.value().handleCPU);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　RTVの生成
//////////////////////////////////////////////////////////////////////////////////////////////////

void DxResource::CreateRTV(const D3D12_RENDER_TARGET_VIEW_DESC& desc) {
	rtvAddress_ = pDxHeap_->GetDescriptorHandle(TYPE_RTV);
	pDevice_->CreateRenderTargetView(cBuffer_.Get(), &desc, rtvAddress_.value().handleCPU);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　swapChainのBufferにResourceを設定する
//////////////////////////////////////////////////////////////////////////////////////////////////

void DxResource::SetSwapChainBuffer(IDXGISwapChain4* swapChain, uint32_t index) {
	HRESULT hr = S_FALSE;
	hr = swapChain->GetBuffer(index, IID_PPV_ARGS(&cBuffer_));
	assert(SUCCEEDED(hr));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　遷移させる
//////////////////////////////////////////////////////////////////////////////////////////////////

void DxResource::Transition(ID3D12GraphicsCommandList* commandList, const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& after) {
	if (befor != bufferState_) {
		Log("now : " + ResourceStateToString(bufferState_) + "\n");
		Log("target : " + ResourceStateToString(befor) + "\n");
		assert("ResourceState MissMatch");
	}
	TransitionResourceState(commandList, cBuffer_.Get(), befor, after);
	bufferState_ = after;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　ViewのGetter
//////////////////////////////////////////////////////////////////////////////////////////////////

const DescriptorHeap::DescriptorHandles& DxResource::GetSRV() const {
	if (srvAddress_ == std::nullopt) {
		assert("not Setting SRV");
	}
	return srvAddress_.value();
}

const DescriptorHeap::DescriptorHandles& DxResource::GetUAV() const {
	if (uavAddress_ == std::nullopt) {
		assert("not Setting UAV");
	}
	return uavAddress_.value();
}

const DescriptorHeap::DescriptorHandles& DxResource::GetRTV() const {
	if (rtvAddress_ == std::nullopt) {
		assert("not Setting RTV");
	}
	return rtvAddress_.value();
}
