#include "ShaderResource.h"

ShaderResource::ShaderResource() {
}

ShaderResource::~ShaderResource() {
}

void ShaderResource::Finalize() {
	cBuffer_.Reset();
	DescriptorHeap::AddFreeSrvList(srvAddress_.assignIndex_);
	DescriptorHeap::AddFreeSrvList(uavAddress_.assignIndex_);
}

void ShaderResource::Init(ID3D12Device* device, DescriptorHeap* dxHeap, 
						  D3D12_RESOURCE_DESC resourceDesc, D3D12_HEAP_PROPERTIES* heapProperties, 
						  const D3D12_HEAP_FLAGS& heapFlags, const D3D12_RESOURCE_STATES& resourceState) {

	assert(device);
	assert(dxHeap);

	pDevice_ = device;
	pDxHeap_ = dxHeap;

	cBuffer_ = CerateShaderResource(device, &resourceDesc, heapProperties, heapFlags, resourceState);
	bufferState_ = resourceState;
}

void ShaderResource::CreateSRV(const D3D12_SHADER_RESOURCE_VIEW_DESC& desc) {
	srvAddress_ = pDxHeap_->AllocateSRV();
	pDevice_->CreateShaderResourceView(cBuffer_.Get(), &desc, srvAddress_.handleCPU);
}

void ShaderResource::CreateUAV(const D3D12_UNORDERED_ACCESS_VIEW_DESC& desc) {
	uavAddress_ = pDxHeap_->AllocateSRV();
	pDevice_->CreateUnorderedAccessView(cBuffer_.Get(), nullptr, &desc, uavAddress_.handleCPU);
}

void ShaderResource::Transition(ID3D12GraphicsCommandList* commandList, const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& after) {
	if (befor != bufferState_) {
		Log("now : " + ResourceStateToString(bufferState_) + "\n");
		Log("target : " + ResourceStateToString(befor) + "\n");
		assert("ResourceState MissMatch");
	}
	TransitionResourceState(commandList, cBuffer_.Get(), befor, after);
	bufferState_ = after;
}
