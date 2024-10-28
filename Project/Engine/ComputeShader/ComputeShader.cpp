#include "ComputeShader.h"

ComputeShader::ComputeShader() {}
ComputeShader::~ComputeShader() {}

void ComputeShader::Finalize() {
	resultResource_.Reset();
	depthOfField_->Finalize();
	gaussianBlur_->Finalize();
	grayScale_->Finalize();
	computeShaderPipelineMap_.clear();
	DescriptorHeap::AddFreeSrvList(uavRenderAddress_.assignIndex_);
	DescriptorHeap::AddFreeSrvList(srvRenderAddress_.assignIndex_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void ComputeShader::Init(ID3D12Device* device, DirectXCompiler* dxCompiler,
						 DescriptorHeap* dxHeap, DescriptorHeap::DescriptorHandles resourceAddress,
						 Shader* shader) {
	device_ = device;
	dxCompiler_ = dxCompiler;
	dxHeap_ = dxHeap;
	runCsResourceAddress_ = resourceAddress;

	// 使用するGPUのグループ
	groupCountX_ = (kWindowWidth_ + 16 - 1) / 16;
	groupCountY_= (kWindowHeight_ + 16 - 1) / 16;

	// PSOの作成
	computeShaderPipelineMap_[CsPipelineType::GrayScale_Pipeline] = std::make_unique<ComputeShaderPipeline>();
	computeShaderPipelineMap_[CsPipelineType::HorizontalBlur_Pipeline] = std::make_unique<ComputeShaderPipeline>();
	computeShaderPipelineMap_[CsPipelineType::VerticalBlur_Pipeline] = std::make_unique<ComputeShaderPipeline>();
	computeShaderPipelineMap_[CsPipelineType::DepthOfField_Pipeline] = std::make_unique<ComputeShaderPipeline>();
	computeShaderPipelineMap_[CsPipelineType::Skinning_Pipeline] = std::make_unique<ComputeShaderPipeline>();
	computeShaderPipelineMap_[CsPipelineType::Blend_Pipeline] = std::make_unique<ComputeShaderPipeline>();
	computeShaderPipelineMap_[CsPipelineType::Result_Pipeline] = std::make_unique<ComputeShaderPipeline>();

	computeShaderPipelineMap_[CsPipelineType::GrayScale_Pipeline]->Init(device, dxCompiler, dxHeap, shader->GetCsShaderData(Shader::GrayScale), RootSignatureType::ComputeShader);
	computeShaderPipelineMap_[CsPipelineType::HorizontalBlur_Pipeline]->Init(device, dxCompiler, dxHeap, shader->GetCsShaderData(Shader::HorizontalBlur), RootSignatureType::ComputeShader);
	computeShaderPipelineMap_[CsPipelineType::VerticalBlur_Pipeline]->Init(device, dxCompiler, dxHeap, shader->GetCsShaderData(Shader::VerticalBlur), RootSignatureType::ComputeShader);
	computeShaderPipelineMap_[CsPipelineType::DepthOfField_Pipeline]->Init(device, dxCompiler, dxHeap, shader->GetCsShaderData(Shader::DepthOfField), RootSignatureType::ComputeShader);
	computeShaderPipelineMap_[CsPipelineType::Skinning_Pipeline]->Init(device, dxCompiler, dxHeap, shader->GetCsShaderData(Shader::SkinningCS), RootSignatureType::CsSkinning);
	computeShaderPipelineMap_[CsPipelineType::Blend_Pipeline]->Init(device, dxCompiler, dxHeap, shader->GetCsShaderData(Shader::Blend), RootSignatureType::ComputeShaderBlend);
	computeShaderPipelineMap_[CsPipelineType::Result_Pipeline]->Init(device, dxCompiler, dxHeap, shader->GetCsShaderData(Shader::Result), RootSignatureType::CSReultRenderBlend);

	// postEffectの作成
	grayScale_ = std::make_unique<GrayScale>(groupCountX_, groupCountY_, computeShaderPipelineMap_[CsPipelineType::GrayScale_Pipeline].get());
	gaussianBlur_ = std::make_unique<GaussianBlur>(groupCountX_, groupCountY_,
												   computeShaderPipelineMap_[CsPipelineType::HorizontalBlur_Pipeline].get(),
												   computeShaderPipelineMap_[CsPipelineType::VerticalBlur_Pipeline].get()
	);
	depthOfField_ = std::make_unique<DepthOfField>(groupCountX_, groupCountY_, computeShaderPipelineMap_[CsPipelineType::DepthOfField_Pipeline].get());

	gaussianBlur_->Init(device_, dxHeap_);
	grayScale_->Init(device_, dxHeap_);
	depthOfField_->Init(device_, dxHeap_);

	grayScale_->CreateSRV();
	gaussianBlur_->CreateSRV();
	depthOfField_->CreateSRV();

	// ----------------------------------------------------------------------
	// ↓ CSの内容を合成するResourceの作成
	// ----------------------------------------------------------------------
	CreateBlendResource();
}

void ComputeShader::SetSkinningPipeline(const CsPipelineType& type, ID3D12GraphicsCommandList* commandList) {
	computeShaderPipelineMap_[type]->SetPipelineState(commandList);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　computerShaderを実行する
//////////////////////////////////////////////////////////////////////////////////////////////////

void ComputeShader::RunComputeShader(ID3D12GraphicsCommandList* commandList) {
	if (isCsReset_) {
		executeCsArray_.clear();
		isCsReset_ = false;
	}

	isRun_ = true;
	if (executeCsArray_.empty()) {
		isRun_ = false;
		return;
	}
	// 先頭の配列に元となるResourceのSRVを指定する
	executeCsArray_[0]->SetReferenceResourceHandles(runCsResourceAddress_);

	for (uint32_t oi = 0; oi < executeCsArray_.size(); oi++) {
		// ----------------------------------------------------------------------
		// ↓ 各クラスでDispatchを行う
		// ----------------------------------------------------------------------
		executeCsArray_[oi]->ConfigureResource(commandList);

		// ----------------------------------------------------------------------
		// ↓ 次に行うエフェクトがあればその参照するResourceを今行った物に変更する
		// ----------------------------------------------------------------------
		if (oi < executeCsArray_.size() - 1) {
			executeCsArray_[oi + 1]->SetReferenceResourceHandles(executeCsArray_[oi]->GetLastIndexSRVHandle());
		}
	}

	// ----------------------------------------------------------------------
	// ↓ 最終的に描画するResourceに加工したTextureを移す
	// ----------------------------------------------------------------------
	computeShaderPipelineMap_[CsPipelineType::Blend_Pipeline]->SetPipelineState(commandList);
	uint32_t lastIndex = static_cast<uint32_t>(executeCsArray_.size()) - 1;
	executeCsArray_[lastIndex]->ConfigureResultSRVResource(commandList);
	//depthOfField_->ConfigureResultSRVResource(commandList);
	commandList->SetComputeRootDescriptorTable(1, uavRenderAddress_.handleGPU);
	//Log("------------------ Dispatch!!!!! ----------------------\n");
	commandList->Dispatch(groupCountX_, groupCountY_, 1);

	// ----------------------------------------------------------------------
	// ↓ 使用したエフェクトのResourceを元に戻す
	// ----------------------------------------------------------------------
	for (uint32_t oi = 0; oi < executeCsArray_.size(); oi++) {
		executeCsArray_[oi]->TransitionAllResourceHandles(commandList);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　最終結果として描画するTextureを作成する
//////////////////////////////////////////////////////////////////////////////////////////////////

void ComputeShader::BlendRenderTarget(ID3D12GraphicsCommandList* commandList, const D3D12_GPU_DESCRIPTOR_HANDLE& spriteGpuHandle, const D3D12_GPU_DESCRIPTOR_HANDLE& rendrerGpuHandle) {
	computeShaderPipelineMap_[CsPipelineType::Result_Pipeline]->SetPipelineState(commandList);
	if (!isRun_) {
		commandList->SetComputeRootDescriptorTable(0, runCsResourceAddress_.handleGPU);
	} else {
		TransitionResourceState(commandList, resultResource_.Get(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
		commandList->SetComputeRootDescriptorTable(0, srvRenderAddress_.handleGPU);
	}


	commandList->SetComputeRootDescriptorTable(1, spriteGpuHandle);
	commandList->SetComputeRootDescriptorTable(2, rendrerGpuHandle);
	//Log("------------------ Dispatch!!!!! ----------------------\n");
	commandList->Dispatch(groupCountX_, groupCountY_, 1);


	if (isRun_) {
		TransitionResourceState(commandList, resultResource_.Get(), D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　配列にCsを設定する
//////////////////////////////////////////////////////////////////////////////////////////////////

void ComputeShader::SetComputeShader(const CSKind& kind) {
	switch (kind) {
	case CSKind::GrayScale:
		executeCsArray_.push_back(grayScale_.get());
		break;

	case CSKind::GaussianBlue:
		executeCsArray_.push_back(gaussianBlur_.get());
		break;
	default:
		assert("not exist effect");
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　最終的に描画させるResourceの作成
//////////////////////////////////////////////////////////////////////////////////////////////////
void ComputeShader::CreateBlendResource() {
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
	resultResource_ = CerateShaderResource(device_, &desc, &heapProperties, D3D12_HEAP_FLAG_ALLOW_DISPLAY, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

	// ------------------------------------------------------------
	// UAVの設定
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Format = resultResource_->GetDesc().Format;
	// SRVを作成するDescriptorHeapの場所を求める
	//uavRenderAddress_ = dxHeap_->GetDescriptorHandle(DescriptorHeapType::TYPE_SRV);
	uavRenderAddress_ = dxHeap_->AllocateSRV();
	// 生成
	device_->CreateUnorderedAccessView(resultResource_.Get(), nullptr, &uavDesc, uavRenderAddress_.handleCPU);

	// ------------------------------------------------------------
	// SRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	// SRVを作成するDescriptorHeapの場所を求める
	srvRenderAddress_ = dxHeap_->AllocateSRV();
	// 生成
	device_->CreateShaderResourceView(resultResource_.Get(), &srvDesc, srvRenderAddress_.handleCPU);
}