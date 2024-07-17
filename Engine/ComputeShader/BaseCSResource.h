#pragma once
#include "DescriptorHeap.h"
#include "DirectXUtils.h"
#ifdef _DEBUG
#include "ImGuiManager.h"
#endif

class BaseCSResource {
public:

	BaseCSResource() = default;
	virtual ~BaseCSResource() = default;

	void Finalize();

	virtual void Init(ID3D12Device* device, DescriptorHeap* dxHeap);

	virtual void Draw(ID3D12GraphicsCommandList* commandList);

	void TransitionUAVResource(ID3D12GraphicsCommandList* commandList, const D3D12_RESOURCE_STATES& beforState, const D3D12_RESOURCE_STATES& afterState);

	const DescriptorHeap::DescriptorHandles GetSRVHandle() const { return srvAddress_; }

protected:

	// dxHeap
	DescriptorHeap* dxHeap_ = nullptr;
	// device
	ID3D12Device* device_ = nullptr;

	// resource
	ComPtr<ID3D12Resource> uavBuffer_ = nullptr;
	ComPtr<ID3D12Resource> cBuffer_ = nullptr;

	// viewHandle
	DescriptorHeap::DescriptorHandles uavAddress_;
	DescriptorHeap::DescriptorHandles srvAddress_;
	
};

