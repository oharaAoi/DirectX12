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

enum class CSKind {
	GrayScale,
	GaussianBlue
};

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
			  DescriptorHeap* dxHeap, DescriptorHeap::DescriptorHandles resourceAddress, 
			  Shader* shader);

	void SetSkinningPipeline(const CsPipelineType& type, ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 行うCsをセットする
	/// </summary>
	/// <param name="kind"></param>
	void SetComputeShader(const CSKind& kind);

	/// <summary>
	/// 行うCSをReset
	/// </summary>
	void ResetComputeShader() { isCsReset_ = true; }

	/// <summary>
	/// 最終的に描画するResourceを作成する
	/// </summary>
	void CreateBlendResource();

	/// <summary>
	/// computerShaderを実行する
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	void RunComputeShader(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 最終結果として描画するTextureを作成する
	/// </summary>
	/// <param name="commandList">: コマンドリスト</param>
	/// <param name="spriteGpuHandle">: sprite描画のGPUハンドル</param>
	/// <param name="rendrerGpuHandle">: 最終描画するTextureのGPUハンドル</param>
	void BlendRenderTarget(ID3D12GraphicsCommandList* commandList, const D3D12_GPU_DESCRIPTOR_HANDLE& spriteGpuHandle, const D3D12_GPU_DESCRIPTOR_HANDLE& rendrerGpuHandle);

	const bool GetIsRun() const { return isRun_; }

private:
	// computeShader用のパイプライン
	std::unordered_map<CsPipelineType, std::unique_ptr<ComputeShaderPipeline>> computeShaderPipelineMap_;

	// postEffect
	std::unique_ptr<GrayScale> grayScale_;
	std::unique_ptr<GaussianBlur> gaussianBlur_;
	std::unique_ptr<DepthOfField> depthOfField_;

	ComPtr<ID3D12Resource> resultResource_;
	
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

	// Cs実行時最初に参照するアドレス
	DescriptorHeap::DescriptorHandles runCsResourceAddress_;

	UINT groupCountX_;
	UINT groupCountY_;

	bool isCsReset_;
	bool isRun_;
};

