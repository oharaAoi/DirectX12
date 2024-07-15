#pragma once
// DirectX
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <memory>
#include <vector>
// PSO
#include "RootSignature.h"
#include "InputLayout.h"
#include "DirectXCompiler.h"
#include "Shader.h"
#include "Blend.h"

class ComputeShaderPipeline {
public:

	ComputeShaderPipeline();
	~ComputeShaderPipeline();

	void Init(ID3D12Device* device, const std::string& shaderPath);

private:

	// rootSignature
	std::unique_ptr<RootSignature> rootSignature_ = nullptr;

	// inputLayout
	InputLayout inputLayout_;
	std::vector<D3D12_INPUT_ELEMENT_DESC> elementDescs = {};

	// Shader
	ComPtr<IDxcBlob> computeShaderBlob_ = nullptr;

	// PSO
	ComPtr<ID3D12PipelineState> csPipelineState_ = nullptr;

	// DXCで使う
	DirectXCompiler* dxCompiler_ = nullptr;

	// device
	ID3D12Device* device_ = nullptr;

	// Blend
	Blend blend_;

};

