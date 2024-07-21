#pragma once
#include "DescriptorHeap.h"
#include "DirectXUtils.h"
#include "MyMath.h"
#include "ComputeShaderPipeline.h"
#ifdef _DEBUG
#include "ImGuiManager.h"
#endif

class BaseCSResource {
public:

	struct UavBufferData {
		ComPtr<ID3D12Resource> uavBuffer = nullptr;
		DescriptorHeap::DescriptorHandles uavAddress;
		//DescriptorHeap::DescriptorHandles srvAddress;
	};


public:

	BaseCSResource() = default;
	virtual ~BaseCSResource() = default;

	void Finalize();

	virtual void Init(ID3D12Device* device, DescriptorHeap* dxHeap);

	virtual void SetResource(ID3D12GraphicsCommandList* commandList);

	void TransitionUAVResource(ID3D12GraphicsCommandList* commandList, const D3D12_RESOURCE_STATES& beforState, const D3D12_RESOURCE_STATES& afterState, const uint32_t& index);

	const DescriptorHeap::DescriptorHandles GetSRVHandle(const uint32_t& index) const { return uavBuffers_[index].uavAddress; }

	/// <summary>
	/// Resourceを作成する
	/// </summary>
	/// <param name="createNum">何個作成するか</param>
	void CreateResourceBuffer(const uint32_t& createNum);

	ComPtr<ID3D12Resource> GetFinalUAVBuffer() { return uavBuffers_.back().uavBuffer; }

	/// <summary>
	/// 参照するResourceのHandleを設定する
	/// </summary>
	/// <param name="handle"></param>
	void SetReferenceResourceHandles(const DescriptorHeap::DescriptorHandles& handle) {
		referenceResourceHandles_ = handle;
	}

	CsPipelineType GetUsePipeline() const { return usePipelineType_; }

protected:

	// dxHeap
	DescriptorHeap* dxHeap_ = nullptr;
	// device
	ID3D12Device* device_ = nullptr;

	// resource
	ComPtr<ID3D12Resource> cBuffer_ = nullptr;
	std::vector<UavBufferData> uavBuffers_;

	// Handle
	DescriptorHeap::DescriptorHandles referenceResourceHandles_;
	DescriptorHeap::DescriptorHandles writeResourceHandles_;

	// 
	CsPipelineType usePipelineType_;
};

