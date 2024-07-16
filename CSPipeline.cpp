#include "CSPipeline.h"

CSPipeline::CSPipeline() {
}

CSPipeline::~CSPipeline() {
}

void CSPipeline::Finalize() {
	rootSignature_->Finalize();
	csPipelineState_.Reset();
	computeShaderBlob_.Reset();
}

/// <summary>
/// 初期化関数
/// </summary>
/// <param name="device">デバイス</param>
/// <param name="dxCompiler">コンパイラー</param>
/// <param name="dxHeap">descriptorHeap</param>
/// <param name="computeShaderPath">シェーダーのパス</param>
void CSPipeline::Init(ID3D12Device* device, DirectXCompiler* dxCompiler, DescriptorHeap* dxHeap, const std::string& computeShaderPath) {
	device_ = device;
	dxCompiler_ = dxCompiler;
	dxHeap_ = dxHeap;

	// rootSignatureの作成
	rootSignature_ = std::make_unique<RootSignature>(device_, RootSignatureType::ComputeShader);
	// shaderを読む
	computeShaderBlob_ = dxCompiler_->CsShaderCompile(computeShaderPath);

	// PSOの生成
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
	desc.pRootSignature = rootSignature_->GetRootSignature();
	desc.CS = { reinterpret_cast<BYTE*>(computeShaderBlob_->GetBufferPointer()),computeShaderBlob_->GetBufferSize() };

	HRESULT hr = device_->CreateComputePipelineState(&desc, IID_PPV_ARGS(&csPipelineState_));
	assert(SUCCEEDED(hr));
}

void CSPipeline::SetPipelineState(ID3D12GraphicsCommandList* commandList) {
	// rootSignatureとpipelineを設定する
	commandList->SetComputeRootSignature(rootSignature_->GetRootSignature());
	commandList->SetPipelineState(csPipelineState_.Get());
}
