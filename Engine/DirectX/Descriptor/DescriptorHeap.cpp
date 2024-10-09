#include "DescriptorHeap.h"

DescriptorHeap::DescriptorHeap(ID3D12Device* device) {
	Initialize(device);
}

DescriptorHeap::~DescriptorHeap() {
}

void DescriptorHeap::Initialize(ID3D12Device* device) {
	assert(device);
	device_ = device;

	if (!descriptorSize_) {
		descriptorSize_ = std::make_unique<DescriptorSize>(
			device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV),
			device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV),
			device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV)
		);
	}

	// ヒープの生成
	rtvHeap_ = CreateDescriptorHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 7, false);
	srvHeap_ = CreateDescriptorHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 128, true);
	dsvHeap_ = CreateDescriptorHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 2, false);

	useSrvIndex_ = 0;	// SRVの先頭はImGuiで使うため0から始める
	useDsvIndex_ = -1;	// 他は生成時に+1しているため一番最初ように-1から始める
	useRtvIndex_ = -1;
}

void DescriptorHeap::Finalize() {
	rtvHeap_.Reset();
	srvHeap_.Reset();
	dsvHeap_.Reset();
}

void DescriptorHeap::SetSRVHeap(ID3D12GraphicsCommandList* commandList) {
	ID3D12DescriptorHeap* descriptorHeaps[] = { srvHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
}

DescriptorHeap::DescriptorHandles DescriptorHeap::GetDescriptorHandle(const DescriptorHeapType& type) {
	DescriptorHandles handle{};

	switch (type) {
	case DescriptorHeapType::TYPE_SRV:
		handle.handleCPU = GetCPUDescriptorHandle(srvHeap_.Get(), descriptorSize_->GetSRV(), (static_cast<int>(useSrvIndex_) + 1));
		handle.handleGPU = GetGPUDescriptorHandle(srvHeap_.Get(), descriptorSize_->GetSRV(), (static_cast<int>(useSrvIndex_) + 1));
		useSrvIndex_++;
		break;

	case DescriptorHeapType::TYPE_DSV:
		handle.handleCPU = GetCPUDescriptorHandle(dsvHeap_.Get(), descriptorSize_->GetDSV(), (static_cast<int>(useDsvIndex_) + 1));
		//handle.handleGPU = GetGPUDescriptorHandle(dsvHeap_.Get(), descriptorSize_->GetDSV(), (static_cast<int>(useDsvIndex_) + 1));
		useDsvIndex_++;
		break;

	case DescriptorHeapType::TYPE_RTV:
		handle.handleCPU = GetCPUDescriptorHandle(rtvHeap_.Get(), descriptorSize_->GetRTV(), (static_cast<int>(useRtvIndex_) + 1));
		//handle.handleGPU = GetGPUDescriptorHandle(rtvHeap_.Get(), descriptorSize_->GetRTV(), (static_cast<int>(useRtvIndex_) + 1));
		useRtvIndex_++;
		break;
	}

	return handle;
}
