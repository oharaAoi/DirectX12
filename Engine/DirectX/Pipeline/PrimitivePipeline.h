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
#include "Engine/Utilities/Shader.h"

class PrimitivePipeline {
public:

	PrimitivePipeline();
	~PrimitivePipeline();

	void Init(ID3D12Device* device, DirectXCompiler* dxCompiler, const Shader::ShaderData& shader);

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
	void ShaderCompile(const std::string& vertexShader, const std::string& pixelShader);

	/// <summary>
	/// BlendStateの設定
	/// </summary>
	D3D12_BLEND_DESC SetBlendState();

	/// <summary>
	/// RasterizerStateの設定
	/// </summary>
	D3D12_RASTERIZER_DESC SetRasterizerState();

	/// <summary>
	/// DepthStencilStateの設定
	/// </summary>
	/// <returns></returns>
	D3D12_DEPTH_STENCIL_DESC SetDepthStencilState();

	/// <summary>
	/// PSOの生成
	/// </summary>
	void CreatePSO();

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
};