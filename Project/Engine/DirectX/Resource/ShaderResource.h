#pragma once
#include "Engine/Utilities/DirectXUtils.h"
#include "Engine/DirectX/Descriptor/DescriptorHeap.h"

class ShaderResource {
public:

	ShaderResource();
	~ShaderResource();

	void Finalize();

	void Init(ID3D12Device* device, DescriptorHeap* dxHeap,
			  D3D12_RESOURCE_DESC* resourceDesc, D3D12_HEAP_PROPERTIES* heapProperties,
			  const D3D12_HEAP_FLAGS& heapFlags, const D3D12_RESOURCE_STATES& resourceState);

	void CreateSRV(const D3D12_SHADER_RESOURCE_VIEW_DESC& desc);
	void CreateUAV(const D3D12_UNORDERED_ACCESS_VIEW_DESC& desc);

	void Transition(ID3D12GraphicsCommandList* commandList, const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& after);

public:

	ID3D12Resource* GetResource() { return cBuffer_.Get(); }

	const D3D12_RESOURCE_STATES& GetState() const { return bufferState_; }

	const DescriptorHeap::DescriptorHandles& GetSRV() const { return srvAddress_; }
	const DescriptorHeap::DescriptorHandles& GetUAV() const { return uavAddress_; }

private:

	ID3D12Device* pDevice_ = nullptr;
	DescriptorHeap* pDxHeap_ = nullptr;

	ComPtr<ID3D12Resource> cBuffer_;
	D3D12_RESOURCE_STATES bufferState_;

	DescriptorHeap::DescriptorHandles uavAddress_;
	DescriptorHeap::DescriptorHandles srvAddress_;
};

