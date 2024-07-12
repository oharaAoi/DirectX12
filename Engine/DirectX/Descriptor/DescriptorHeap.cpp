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
	rtvHeap_ = CreateDescriptorHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 3, false);
	srvHeap_ = CreateDescriptorHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 128, true);
	dsvHeap_ = CreateDescriptorHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);
}

void DescriptorHeap::Finalize() {
	rtvHeap_.Reset();
	srvHeap_.Reset();
	dsvHeap_.Reset();
}
