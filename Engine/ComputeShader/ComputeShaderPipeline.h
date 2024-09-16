#pragma once
// DirectX
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <memory>
#include <vector>
// PSO
#include "Engine/DirectX/RootSignature/RootSignature.h"
#include "Engine/DirectX/DirectXCompiler/DirectXCompiler.h"
#include "Engine/DirectX/Descriptor/DescriptorHeap.h"
#include "Engine/DirectX/Descriptor/DescriptorSize.h"
#include "Engine/Utilities/Shader.h"

enum class CsPipelineType {
	HorizontalBlur_Pipeline,
	VerticalBlur_Pipeline,
	GrayScale_Pipeline,
	DepthOfField_Pipeline,
	Blend_Pipeline
};

class ComputeShaderPipeline {
public:

	ComputeShaderPipeline();
	~ComputeShaderPipeline();

	void Finalize();

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="dxCompiler">コンパイラー</param>
	/// <param name="dxHeap">descriptorHeap</param>
	/// <param name="computeShaderPath">シェーダーのパス</param>
	void Init(ID3D12Device* device, DirectXCompiler* dxCompiler,
			  DescriptorHeap* dxHeap, const std::string& computeShaderPath,
			  const CsPipelineType& type
	);

	/// <summary>
	/// パイプラインを設定する
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	void SetPipelineState(ID3D12GraphicsCommandList* commandList);

private:

	// rootSignature
	std::unique_ptr<RootSignature> rootSignature_ = nullptr;
	// Shader
	ComPtr<IDxcBlob> computeShaderBlob_ = nullptr;
	// PSO
	ComPtr<ID3D12PipelineState> csPipelineState_ = nullptr;

	// DXCで使う
	DirectXCompiler* dxCompiler_ = nullptr;
	// dxHeap
	DescriptorHeap* dxHeap_ = nullptr;
	// device
	ID3D12Device* device_ = nullptr;
};

