#include "ComputeShaderPipeline.h"

ComputeShaderPipeline::ComputeShaderPipeline() {}
ComputeShaderPipeline::~ComputeShaderPipeline() {}

void ComputeShaderPipeline::Finalize() {
}

void ComputeShaderPipeline::Init(ID3D12Device* device, DirectXCompiler* dxCompiler,
								 DescriptorHeap* dxHeap, const std::string& computeShaderPath,
								 const CsPipelineType& type
) {
	device_ = device;
	dxCompiler_ = dxCompiler;
	dxHeap_ = dxHeap;

	// rootSignatureの作成
	if (type == CsPipelineType::Blend_Pipeline) {
		rootSignature_ = std::make_unique<RootSignature>(device_, RootSignatureType::ComputeShaderBlend);
	} else {
		rootSignature_ = std::make_unique<RootSignature>(device_, RootSignatureType::ComputeShader);
	}
	// shaderを読む
	computeShaderBlob_ = dxCompiler_->CsShaderCompile(computeShaderPath);

	// PSOの生成
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
	desc.pRootSignature = rootSignature_->GetRootSignature();
	desc.CS = { reinterpret_cast<BYTE*>(computeShaderBlob_->GetBufferPointer()),computeShaderBlob_->GetBufferSize() };

	HRESULT hr = device_->CreateComputePipelineState(&desc, IID_PPV_ARGS(&csPipelineState_));
	assert(SUCCEEDED(hr));
}

void ComputeShaderPipeline::SetPipelineState(ID3D12GraphicsCommandList* commandList) {
	// rootSignatureとpipelineを設定する
	commandList->SetComputeRootSignature(rootSignature_->GetRootSignature());
	commandList->SetPipelineState(csPipelineState_.Get());
}
