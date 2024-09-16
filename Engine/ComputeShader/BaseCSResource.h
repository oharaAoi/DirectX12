#pragma once
#include "Engine/DirectX/Descriptor/DescriptorHeap.h"
#include "Engine/Utilities/DirectXUtils.h"
#include "Engine/Math/MyMath.h"
#include "Engine/ComputeShader/ComputeShaderPipeline.h"
#include "Engine/Manager/ImGuiManager.h"

class BaseCSResource {
public:

	struct BufferHandleData {
		ComPtr<ID3D12Resource> uavBuffer = nullptr;
		DescriptorHeap::DescriptorHandles uavAddress;
		DescriptorHeap::DescriptorHandles srvAddress;
	};


public:

	BaseCSResource() = default;
	virtual ~BaseCSResource() = default;

	void Finalize();

	virtual void Init(ID3D12Device* device, DescriptorHeap* dxHeap);

	/// <summary>
	/// ResourceをcommandListに登録する
	/// </summary>
	/// <param name="commandList">: コマンドリスト</param>
	virtual void ConfigureResource(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 参照するResourceをコマンドリストに積む
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="handleGPU">参照するResourceのアドレス</param>
	virtual void ConfigureReferenceResource(ID3D12GraphicsCommandList* commandList, const D3D12_GPU_DESCRIPTOR_HANDLE& handleGPU);

	void ConfigureResultSRVResource(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// UAVのResourceの状態を変更する
	/// </summary>
	/// <param name="commandList">: コマンドリスト</param>
	/// <param name="beforState">: 遷移前状態</param>
	/// <param name="afterState">: 遷後後状態</param>
	/// <param name="index">: Resource配列の何番目か</param>
	void TransitionUAVResource(ID3D12GraphicsCommandList* commandList, const D3D12_RESOURCE_STATES& beforState, const D3D12_RESOURCE_STATES& afterState, const uint32_t& index);

	/// <summary>
	/// 配列に入っているすべてのResourceの状態を変更する
	/// </summary>
	/// <param name="commandList"></param>
	void TransitionAllResourceHandles(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// Resourceを作成する
	/// </summary>
	/// <param name="createNum">: 何個作成するか</param>
	void CreateResourceBuffer(const uint32_t& createNum);

	/// <summary>
	/// SRVを作成する
	/// </summary>
	void CreateSRV();

	/// <summary>
	/// 参照するResourceのHandleを設定する
	/// </summary>
	/// <param name="handle">: 参照するHandle</param>
	void SetReferenceResourceHandles(const DescriptorHeap::DescriptorHandles& handle) {
		referenceResourceHandles_ = handle;
	}

	/// <summary>
	/// Resource配列の最後の要素のSRVを返す
	/// </summary>
	/// <returns></returns>
	const DescriptorHeap::DescriptorHandles GetLastIndexSRVHandle() const;

	/// <summary>
	/// 使用するパイプラインを返す
	/// </summary>
	/// <returns> pipelineType</returns>
	CsPipelineType GetUsePipeline() const { return usePipelineType_; }

protected:

	// dxHeap
	DescriptorHeap* dxHeap_ = nullptr;
	// device
	ID3D12Device* device_ = nullptr;

	// resource
	ComPtr<ID3D12Resource> cBuffer_ = nullptr;
	std::vector<BufferHandleData> bufferHandles_;

	// Handle
	DescriptorHeap::DescriptorHandles referenceResourceHandles_;
	DescriptorHeap::DescriptorHandles writeResourceHandles_;

	// 
	CsPipelineType usePipelineType_;
};

