#include "ComputeShaderPipeline.h"

ComputeShaderPipeline::ComputeShaderPipeline() {
	rootSignature_ = std::make_unique<RootSignature>();
}
ComputeShaderPipeline::~ComputeShaderPipeline() { Finalize(); }

void ComputeShaderPipeline::Finalize() {
	rootSignature_->Finalize();
}

void ComputeShaderPipeline::Init(ID3D12Device* device, DirectXCompiler* dxCompiler,
								 DescriptorHeap* dxHeap, const std::string& computeShaderPath,
								 const RootSignatureType& type) {
	device_ = device;
	dxCompiler_ = dxCompiler;
	dxHeap_ = dxHeap;

	rootSignature_->Initialize(device, type);
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
