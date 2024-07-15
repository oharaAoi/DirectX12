#include "ComputeShader.h"

ComputeShader::ComputeShader() {
}

ComputeShader::~ComputeShader() {
}

void ComputeShader::Finalize() {
}

void ComputeShader::Init(ID3D12Device* device, DirectXCompiler* dxCompiler,
						 DescriptorHeap* dxHeap, DescriptorSize* dxSize, ID3D12Resource* textureResource, const std::string& computeShaderPath) {
	device_ = device;
	dxCompiler_ = dxCompiler;
	dxHeap_ = dxHeap;
	dxSize_ = dxSize;
	textureResource_ = textureResource;

	// rootSignatureの作成
	rootSignature_ = std::make_unique<RootSignature>(device_, RootSignatureType::ComputeShader);
	// shaderを読む
	computeShaderBlob_ = dxCompiler_->CsShaderCompile(computeShaderPath);

	// PSOの生成
	D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
	desc.pRootSignature = rootSignature_->GetRootSignature();
	desc.CS = { reinterpret_cast<BYTE*>(computeShaderBlob_->GetBufferPointer()),computeShaderBlob_->GetBufferSize() };

	CreateUAV();

	HRESULT hr = device_->CreateComputePipelineState(&desc, IID_PPV_ARGS(&csPipelineState_));
	assert(SUCCEEDED(hr));
}

void ComputeShader::SetPipelineState(ID3D12GraphicsCommandList* commandList) {
	commandList->SetComputeRootSignature(rootSignature_->GetRootSignature());
	commandList->SetPipelineState(csPipelineState_.Get());

	TextureManager::GetInstance()->SetRenderTexture(commandList, 0);
	commandList->SetComputeRootDescriptorTable(1, uavHandleGPU_);
}

void ComputeShader::CreateUAV() {
	D3D12_RESOURCE_DESC desc{};
	desc.Width = 1280;			// 画面の横幅
	desc.Height = 720;			// 画面の縦幅
	desc.MipLevels = 1;			// 
	desc.DepthOrArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	// HEAPの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	// リソースの作成
	HRESULT hr = device_->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_ALLOW_DISPLAY,
		&desc,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		nullptr,
		IID_PPV_ARGS(&uavBuffer_)
	);

	assert(SUCCEEDED(hr));

	/// 
	
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;;
	uavDesc.Format = uavBuffer_->GetDesc().Format;

	uavHandleCPU_ = GetCPUDescriptorHandle(dxHeap_->GetSRVHeap(), dxSize_->GetSRV(), dxHeap_->GetUseSrvIndex() + 1);
	uavHandleGPU_ = GetGPUDescriptorHandle(dxHeap_->GetSRVHeap(), dxSize_->GetSRV(), dxHeap_->GetUseSrvIndex() + 1);

	device_->CreateUnorderedAccessView(uavBuffer_.Get(), nullptr, &uavDesc, uavHandleCPU_);
}
