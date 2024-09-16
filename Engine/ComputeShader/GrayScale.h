#pragma once
#include "BaseCSResource.h"

class GrayScale :
	public BaseCSResource {

	struct GrayScaleData {
		float grayScaleAmount;// grayScaleの強さ
	};

public:

	GrayScale(const UINT& gpuGroupCountX, const UINT& gpuGroupCountY, ComputeShaderPipeline* pipeline);
	~GrayScale();

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

	const DescriptorHeap::DescriptorHandles GetSRVHandle() const { return bufferHandles_[0].srvAddress; }

	void TransitionResource(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES beforState, D3D12_RESOURCE_STATES afterState);

private:

	GrayScaleData* data_ = nullptr;

	// 使用するGPUのグループ
	UINT groupCountX_;
	UINT groupCountY_;

	ComputeShaderPipeline* pipeline_;
};

