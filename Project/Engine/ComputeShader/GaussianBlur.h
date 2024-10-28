#pragma once
#include "BaseCSResource.h"

class GaussianBlur :
	public BaseCSResource {

	struct GaussianBlurData {
		float blurStrength; // ぼかしの強さ
		float kernelSize;	// カーネルサイズ
		Vector2 direction;
	};

public:

	GaussianBlur(const UINT& gpuGroupCountX, const UINT& gpuGroupCountY,
				 ComputeShaderPipeline* vertical, ComputeShaderPipeline* horizontal);
	~GaussianBlur();

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

	const DescriptorHeap::DescriptorHandles GetSRVHandles() const { return bufferHandles_[0].srvAddress; }

	/// <summary>
	/// 全体にブラーを掛けたResourceのアドレスを返す
	/// </summary>
	/// <returns>: srvHandle</returns>
	const DescriptorHeap::DescriptorHandles GetResultSRVHandle() const { return bufferHandles_[1].srvAddress; }

private:
	// 横ブラーをかけるResource。これもUAV→SRVなどの手順を踏む必要がある
	//DescriptorHeap::DescriptorHandles uavHorizontalAddress_;
	//DescriptorHeap::DescriptorHandles srvHorizontalAddress_;

	GaussianBlurData* data_;

	// 使用するGPUのグループ
	UINT groupCountX_;
	UINT groupCountY_;

	ComputeShaderPipeline* verticalPipeline_;
	ComputeShaderPipeline* horizontalPipeline_;
};

