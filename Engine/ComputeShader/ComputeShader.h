#pragma once
// DirectX
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <memory>
#include <vector>
#include "Engine/Math/MyMath.h"
// PSO
#include "ComputeShaderPipeline.h"
#include "Engine/Manager/TextureManager.h"
#include "Engine/Utilities/Shader.h"
// postEffect
#include "GrayScale.h"
#include "GaussianBlur.h"
#include "DepthOfField.h"

class ComputeShader {
public:

	ComputeShader();
	~ComputeShader();

	void Finalize();

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="dxCompiler">コンパイラー</param>
	/// <param name="dxHeap">descriptorHeap</param>
	/// <param name="computeShaderPath">シェーダーのパス</param>
	void Init(ID3D12Device* device, DirectXCompiler* dxCompiler,
			  DescriptorHeap* dxHeap, DescriptorHeap::DescriptorHandles offScreenResourceAddress, 
			  Shader* shader);

	/// <summary>
	/// 最終的に描画するResourceを作成する
	/// </summary>
	void CreateRenderResource();

	/// <summary>
	/// computerShaderを実行する
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	void RunComputeShader(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// UAVの状態を変更する
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="beforState">変更前の状態</param>
	/// <param name="afterState">変更後の状態</param>
	void TransitionUAVResource(ID3D12GraphicsCommandList* commandList, const D3D12_RESOURCE_STATES& beforState, const D3D12_RESOURCE_STATES& afterState);

	/// <summary>
	/// computeShaderで加工したResourceのアドレスを取得する関数
	/// </summary>
	/// <returns></returns>
	const D3D12_GPU_DESCRIPTOR_HANDLE GetShaderResourceHandleGPU() const { return srvRenderAddress_.handleGPU; }

private:
	// computeShader用のパイプライン
	std::unordered_map<CsPipelineType, std::unique_ptr<ComputeShaderPipeline>> computeShaderPipelineMap_;

	// postEffect
	std::unique_ptr<GrayScale> grayScale_;
	std::unique_ptr<GaussianBlur> gaussianBlur_;
	std::unique_ptr<DepthOfField> depthOfField_;

	ComPtr<ID3D12Resource> renderResource_;

	std::vector<BaseCSResource*> executeCsArray_;
	
	// ---------------------------------------
	// DXCで使う
	DirectXCompiler* dxCompiler_ = nullptr;
	// dxHeap
	DescriptorHeap* dxHeap_ = nullptr;
	// device
	ID3D12Device* device_ = nullptr;

	// ---------------------------------------
	DescriptorHeap::DescriptorHandles uavRenderAddress_;
	DescriptorHeap::DescriptorHandles srvRenderAddress_;

	DescriptorHeap::DescriptorHandles offScreenResourceAddress_;
};

