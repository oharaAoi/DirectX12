#pragma once
#include "BaseCSResource.h"
#include "Engine/DirectX/DirectXCommon/DirectXCommon.h"

class DepthOfField : 
	public BaseCSResource {

	struct DepthData {
		
	};

public:

	DepthOfField(const UINT& gpuGroupCountX, const UINT& gpuGroupCountY, ComputeShaderPipeline* pipeline);
	~DepthOfField();

	void Finalize();

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="device"></param>
	/// <param name="dxHeap"></param>
	void Init(ID3D12Device* device, DescriptorHeap* dxHeap) override;

	/// <summary>
	/// Resourceをコマンドにセットする
	/// </summary>
	/// <param name="commandList"></param>
	void ConfigureResource(ID3D12GraphicsCommandList* commandList) override;

	void TransitionResource(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES beforState, D3D12_RESOURCE_STATES afterState);

	const DescriptorHeap::DescriptorHandles GetSRVHandle() const { return bufferHandles_[0].srvAddress; }

private:

	DepthData* data_;

	// 使用するGPUのグループ
	UINT groupCountX_;
	UINT groupCountY_;

	ComputeShaderPipeline* pipeline_;

	DescriptorHeap::DescriptorHandles copySRVHandles_;


	ID3D12Resource* depthStencilResource_ = nullptr;
	ComPtr<ID3D12Resource> copyDepthStencilResource_ = nullptr;
};

