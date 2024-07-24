#include "ComputeShader.h"

ComputeShader::ComputeShader() {}
ComputeShader::~ComputeShader() {}

void ComputeShader::Finalize() {
	renderResource_.Reset();
	gaussianBlur_->Finalize();
	grayScale_->Finalize();
	computeShaderPipelineMap_.clear();
}

/// <summary>
/// 初期化関数
/// </summary>
/// <param name="device">デバイス</param>
/// <param name="dxCompiler">コンパイラー</param>
/// <param name="dxHeap">descriptorHeap</param>
/// <param name="computeShaderPath">シェーダーのパス</param>
void ComputeShader::Init(ID3D12Device* device, DirectXCompiler* dxCompiler,
						 DescriptorHeap* dxHeap, DescriptorHeap::DescriptorHandles offScreenResourceAddress,
						 Shader* shader) {
	device_ = device;
	dxCompiler_ = dxCompiler;
	dxHeap_ = dxHeap;
	offScreenResourceAddress_ = offScreenResourceAddress;
	
	// PSOの作成
	computeShaderPipelineMap_[CsPipelineType::GrayScale_Pipeline] = std::make_unique<ComputeShaderPipeline>();
	computeShaderPipelineMap_[CsPipelineType::GaussianBlur_Pipeline] = std::make_unique<ComputeShaderPipeline>();
	computeShaderPipelineMap_[CsPipelineType::Blend_Pipeline] = std::make_unique<ComputeShaderPipeline>();

	computeShaderPipelineMap_[CsPipelineType::GrayScale_Pipeline]->Init(device, dxCompiler, dxHeap, shader->GetCsShaderData(Shader::GrayScale), CsPipelineType::GrayScale_Pipeline);
	computeShaderPipelineMap_[CsPipelineType::GaussianBlur_Pipeline]->Init(device, dxCompiler, dxHeap, shader->GetCsShaderData(Shader::GaussianBlur), CsPipelineType::GaussianBlur_Pipeline);
	computeShaderPipelineMap_[CsPipelineType::Blend_Pipeline]->Init(device, dxCompiler, dxHeap, shader->GetCsShaderData(Shader::Blend), CsPipelineType::Blend_Pipeline);

	// postEffectの作成
	grayScale_ = std::make_unique<GrayScale>();
	gaussianBlur_ = std::make_unique<GaussianBlur>();

	gaussianBlur_->Init(device_, dxHeap_);
	grayScale_->Init(device_, dxHeap_);

	grayScale_->CreateSRV();
	gaussianBlur_->CreateSRV();

	// 参照するResourceのHandleを渡す
	grayScale_->SetReferenceResourceHandles(offScreenResourceAddress_);
	gaussianBlur_->SetReferenceResourceHandles(grayScale_->GetSRVHandle());

	//executeCsArray_.push_back(gaussianBlur_.get());
	executeCsArray_.push_back(grayScale_.get());

	// 最終的に描画させるResourceの作成
	D3D12_RESOURCE_DESC desc{};
	desc.Width = kWindowWidth_;			// 画面の横幅
	desc.Height = kWindowHeight_;			// 画面の縦幅
	desc.MipLevels = 1;			// 
	desc.DepthOrArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	// HEAPの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	// Resourceの作成
	renderResource_ = CerateShaderResource(device_, &desc, &heapProperties, D3D12_HEAP_FLAG_ALLOW_DISPLAY, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

	// ------------------------------------------------------------
	// UAVの設定
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Format = renderResource_->GetDesc().Format;
	// SRVを作成するDescriptorHeapの場所を求める
	uavRenderAddress_ = dxHeap_->GetDescriptorHandle(DescriptorHeapType::TYPE_SRV);
	// 生成
	device_->CreateUnorderedAccessView(renderResource_.Get(), nullptr, &uavDesc, uavRenderAddress_.handleCPU);

	// ------------------------------------------------------------
	// SRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	// SRVを作成するDescriptorHeapの場所を求める
	srvRenderAddress_ = dxHeap_->GetDescriptorHandle(DescriptorHeapType::TYPE_SRV);
	// 生成
	device_->CreateShaderResourceView(renderResource_.Get(), &srvDesc, srvRenderAddress_.handleCPU);
}

/// <summary>
/// computerShaderを実行する
/// </summary>
/// <param name="commandList">コマンドリスト</param>
void ComputeShader::RunComputeShader(ID3D12GraphicsCommandList* commandList) {
	UINT groupCountX = (kWindowWidth_ + 16 - 1) / 16;
	UINT groupCountY = (kWindowHeight_ + 16 - 1) / 16;

	for(uint32_t oi = 0; oi < executeCsArray_.size(); oi++){
		// ----------------------------------------------------------------------
		// ↓ Pipelineの設定
		// ----------------------------------------------------------------------
		switch (executeCsArray_[oi]->GetUsePipeline()) {
		case CsPipelineType::GaussianBlur_Pipeline:
			computeShaderPipelineMap_[CsPipelineType::GaussianBlur_Pipeline]->SetPipelineState(commandList);
			break;

		case CsPipelineType::GrayScale_Pipeline:
			computeShaderPipelineMap_[CsPipelineType::GrayScale_Pipeline]->SetPipelineState(commandList);
			break;
		}

		// ----------------------------------------------------------------------
		// ↓ Resourceをコマンドリストに積む
		// ----------------------------------------------------------------------

		executeCsArray_[oi]->SetResource(commandList);
		commandList->Dispatch(groupCountX, groupCountY, 1);

		executeCsArray_[oi]->TransitionUAVResource(commandList, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, 0);
	}

	computeShaderPipelineMap_[CsPipelineType::Blend_Pipeline]->SetPipelineState(commandList);
	uint32_t lastIndex = static_cast<uint32_t>(executeCsArray_.size()) - 1;
	executeCsArray_[lastIndex]->SetResultResource(commandList);
	commandList->SetComputeRootDescriptorTable(1, uavRenderAddress_.handleGPU);

	TransitionResourceState(commandList, renderResource_.Get(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	
	grayScale_->TransitionResource(commandList, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	//gaussianBlur_->TransitionUAVResource(commandList, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, 0);
	//gaussianBlur_->TransitionUAVResource(commandList, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, 1);
}

/// <summary>
/// UAVの状態を読み込みから書き込み状態にする
/// </summary>
/// <param name="commandList">コマンドリスト</param>
void ComputeShader::TransitionUAVResource(ID3D12GraphicsCommandList* commandList, const D3D12_RESOURCE_STATES& beforState, const D3D12_RESOURCE_STATES& afterState) {
	//TransitionResourceState(commandList, uavBuffer_.Get(), beforState, afterState);
	TransitionResourceState(commandList, renderResource_.Get(), beforState, afterState);
}
