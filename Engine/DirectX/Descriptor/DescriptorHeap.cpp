#include "DescriptorHeap.h"

DescriptorHeap::DescriptorHeap(ID3D12Device* device) {
	Initialize(device);
}

DescriptorHeap::~DescriptorHeap() {
}

void DescriptorHeap::Initialize(ID3D12Device* device) {
	assert(device);
	device_ = device;

	// ヒープの生成
	rtvHeap = CreateDescriptorHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false);
	srvHeap = CreateDescriptorHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 128, true);
}

void DescriptorHeap::Finalize() {
	rtvHeap.Reset();
	srvHeap.Reset();
}
