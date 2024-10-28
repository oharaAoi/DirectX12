#pragma once
// DirectX
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <memory>
#include <vector>
// PSO
#include "Engine/DirectX/RootSignature/RootSignature.h"
#include "InputLayout.h"
#include "Engine/DirectX/DirectXCompiler/DirectXCompiler.h"
#include "Engine/DirectX/RTV/RenderTarget.h"
#include "Engine/Utilities/Shader.h"
#include "Blend.h"

class Pipeline {
public:

	Pipeline();
	~Pipeline();

	void Initialize(ID3D12Device* device, DirectXCompiler* dxCompiler, 
					const Shader::ShaderData& shaderData, const RootSignatureType& rootSignatureType,
					const std::vector<D3D12_INPUT_ELEMENT_DESC>& desc, const Blend::BlendMode& blendMode,
					bool isCulling, bool isDepth
	);

	void Draw(ID3D12GraphicsCommandList* commandList);

	void Finalize();

public:

	/// <summary>
	/// inputLayoutの生成
	/// </summary>
	/// <param name="elementDesc">要素の配列</param>
	/// <returns></returns>
	D3D12_INPUT_LAYOUT_DESC CreateInputLayout(const std::vector<D3D12_INPUT_ELEMENT_DESC>& elementDesc);

	/// <summary>
	/// Shaderをcompileする
	/// </summary>
	void ShaderCompile(const Shader::ShaderData& shaderData);

	/// <summary>
	/// RasterizerStateの設定
	/// </summary>
	D3D12_RASTERIZER_DESC SetRasterizerState(bool isCulling);

	/// <summary>
	/// DepthStencilStateの設定
	/// </summary>
	/// <returns></returns>
	D3D12_DEPTH_STENCIL_DESC SetDepthStencilState(bool isDepth);

	/// <summary>
	/// PSOの生成
	/// </summary>
	void CreatePSO(const Blend::BlendMode& blendMode, bool isCulling, bool isDepth);

private:
	// rootSignature
	std::unique_ptr<RootSignature> rootSignature_ = nullptr;
	// inputLayout
	InputLayout inputLayout_;
	std::vector<D3D12_INPUT_ELEMENT_DESC> elementDescs = {};

	// Shader
	ComPtr<IDxcBlob> vertexShaderBlob_ = nullptr;
	ComPtr<IDxcBlob> pixelShaderBlob_ = nullptr;

	// PSO
	ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc_;

	// DXCで使う
	DirectXCompiler* dxCompiler_ = nullptr;

	// device
	ID3D12Device* device_ = nullptr;

	// Blend
	Blend blend_;
};