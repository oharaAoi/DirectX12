#pragma once
#include "DescriptorHeap.h"
#include "DirectXUtils.h"
#include "MyMath.h"
#ifdef _DEBUG
#include "ImGuiManager.h"
#endif

class BaseCSResource {
public:

	struct UavBufferData {
		ComPtr<ID3D12Resource> uavBuffer = nullptr;
		DescriptorHeap::DescriptorHandles uavAddress;
		DescriptorHeap::DescriptorHandles srvAddress;
	};


public:

	BaseCSResource() = default;
	virtual ~BaseCSResource() = default;

	void Finalize();

	virtual void Init(ID3D12Device* device, DescriptorHeap* dxHeap);

	void SetResource(ID3D12GraphicsCommandList* commandList, const uint32_t& index, const uint32_t& rootParameter);

	void TransitionUAVResource(ID3D12GraphicsCommandList* commandList, const D3D12_RESOURCE_STATES& beforState, const D3D12_RESOURCE_STATES& afterState, const uint32_t& index);

	const DescriptorHeap::DescriptorHandles GetSRVHandle(const uint32_t& index) const { return uavBuffers_[index].srvAddress; }

	void CreateUAVBuffer(const uint32_t& createNum);

	ComPtr<ID3D12Resource> GetFinalUAVBuffer() { return uavBuffers_.back().uavBuffer; }

protected:

	// dxHeap
	DescriptorHeap* dxHeap_ = nullptr;
	// device
	ID3D12Device* device_ = nullptr;

	// resource
	/*ComPtr<ID3D12Resource> uavBuffer_ = nullptr;*/
	ComPtr<ID3D12Resource> cBuffer_ = nullptr;

	std::vector<UavBufferData> uavBuffers_;

	// viewHandle
	/*DescriptorHeap::DescriptorHandles uavAddress_;
	DescriptorHeap::DescriptorHandles srvAddress_;*/
	
};

