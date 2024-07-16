#include "ComputeShader.h"

ComputeShader::ComputeShader() {
}

ComputeShader::~ComputeShader() {
}

void ComputeShader::Finalize() {
	uavBuffer_.Reset();
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

	// リソースの作成
	CreateUAV();
}

/// <summary>
/// computerShaderを実行する
/// </summary>
/// <param name="commandList">コマンドリスト</param>
void ComputeShader::RunComputeShader(ID3D12GraphicsCommandList* commandList) {
	// pipelineStateを設定する
	computeShaderPipeline_->SetPipelineState(commandList);
	// 参照するtextureを設定
	commandList->SetComputeRootDescriptorTable(0, offScreenResourceAddress_.handleGPU);
	// 実際に書き込むtextureを設定
	commandList->SetComputeRootDescriptorTable(1, uavAddress_.handleGPU);

	UINT groupCountX = (kWindowWidth_ + 16 - 1) / 16;
	UINT groupCountY = (kWindowHeight_ + 16 - 1) / 16;

	commandList->Dispatch(groupCountX, groupCountY, 1);
}

void ComputeShader::CreateUAV() {
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
	uavBuffer_ = CerateShaderResource(device_, &desc, &heapProperties, D3D12_HEAP_FLAG_ALLOW_DISPLAY, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

	// ------------------------------------------------------------
	// UAVの設定
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Format = uavBuffer_->GetDesc().Format;
	// SRVを作成するDescriptorHeapの場所を求める
	uavAddress_ = dxHeap_->GetDescriptorHandle(DescriptorHeapType::TYPE_SRV);
	// 生成
	device_->CreateUnorderedAccessView(uavBuffer_.Get(), nullptr, &uavDesc, uavAddress_.handleCPU);

	// ------------------------------------------------------------
	// SRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	// SRVを作成するDescriptorHeapの場所を求める
	srvAddress_ = dxHeap_->GetDescriptorHandle(DescriptorHeapType::TYPE_SRV);
	// 生成
	device_->CreateShaderResourceView(uavBuffer_.Get(), &srvDesc, srvAddress_.handleCPU);
}

/// <summary>
/// UAVの状態を読み込みから書き込み状態にする
/// </summary>
/// <param name="commandList">コマンドリスト</param>
void ComputeShader::TransitionUAVResource(ID3D12GraphicsCommandList* commandList, const D3D12_RESOURCE_STATES& beforState, const D3D12_RESOURCE_STATES& afterState) {
	TransitionResourceState(commandList, uavBuffer_.Get(), beforState, afterState);
}
