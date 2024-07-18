#include "ComputeShader.h"

ComputeShader::ComputeShader() {}
ComputeShader::~ComputeShader() {}

void ComputeShader::Finalize() {
	renderResource_.Reset();
	gaussianBlur_->Finalize();
	grayScale_->Finalize();
	computeShaderPipeline_->Finalize();
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
						 const std::string& computeShaderPath) {
	device_ = device;
	dxCompiler_ = dxCompiler;
	dxHeap_ = dxHeap;
	offScreenResourceAddress_ = offScreenResourceAddress;
	
	// PSOの作成
	computeShaderPipeline_ = std::make_unique<ComputeShaderPipeline>();
	computeShaderPipeline_->Init(device, dxCompiler, dxHeap, computeShaderPath);

	// postEffectの作成
	grayScale_ = std::make_unique<GrayScale>();
	gaussianBlur_ = std::make_unique<GaussianBlur>();

	grayScale_->Init(device_, dxHeap_);
	gaussianBlur_->Init(device_, dxHeap_);

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
	renderResource_ = CerateShaderResource(device_, &desc, &heapProperties, D3D12_HEAP_FLAG_ALLOW_DISPLAY, D3D12_RESOURCE_STATE_COPY_DEST);

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

	// ----------------------------------------------------------------------
	// ↓ グレースケール
	// ----------------------------------------------------------------------
	//// pipelineStateを設定する
	//computeShaderPipeline_->SetPipelineState(commandList);
	//// 参照するtextureを設定
	//commandList->SetComputeRootDescriptorTable(0, offScreenResourceAddress_.handleGPU);
	//// 画面に写るリソースとCSに送る定数バッファをコマンドに積む
	//grayScale_->SetResource(commandList);

	//commandList->Dispatch(groupCountX, groupCountY, 1);

	// ----------------------------------------------------------------------
	// ↓ ガウシアンブラー
	// ----------------------------------------------------------------------
	// 水平
	computeShaderPipeline_->SetPipelineState(commandList);
	commandList->SetComputeRootDescriptorTable(0, offScreenResourceAddress_.handleGPU);
	gaussianBlur_->HorizontalSetResource(commandList);
	commandList->Dispatch(groupCountX, groupCountY, 1);

	gaussianBlur_->TransitionUAVResource(commandList, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, 0);

	// 垂直
	//commandList->SetComputeRootDescriptorTable(0, offScreenResourceAddress_.handleGPU);
	gaussianBlur_->VerticalSetResource(commandList);
	commandList->Dispatch(groupCountX, groupCountY, 1);

	gaussianBlur_->TransitionUAVResource(commandList, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE, 1);

	// CSで加工したResourceをコピーするために状態をコピー可能(コピー元)にする
	//grayScale_->TransitionResource(commandList, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE);
	// renderResourceに加工したResourceをコピーする
	commandList->CopyResource(renderResource_.Get(), gaussianBlur_->GetFinalUAVBuffer().Get());
	// リソースの状態をコピー先からShaderResourceにする
	TransitionResourceState(commandList, renderResource_.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	// 次のフレームで読み書きできる状態になっているようにする
	//grayScale_->TransitionResource(commandList, D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

	gaussianBlur_->TransitionUAVResource(commandList, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, 0);
	gaussianBlur_->TransitionUAVResource(commandList, D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, 1);
}

/// <summary>
/// UAVの状態を読み込みから書き込み状態にする
/// </summary>
/// <param name="commandList">コマンドリスト</param>
void ComputeShader::TransitionUAVResource(ID3D12GraphicsCommandList* commandList, const D3D12_RESOURCE_STATES& beforState, const D3D12_RESOURCE_STATES& afterState) {
	//TransitionResourceState(commandList, uavBuffer_.Get(), beforState, afterState);
	TransitionResourceState(commandList, renderResource_.Get(), beforState, afterState);
}
