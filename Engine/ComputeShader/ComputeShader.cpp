#include "ComputeShader.h"

ComputeShader::ComputeShader() {
}

ComputeShader::~ComputeShader() {
}

void ComputeShader::Finalize() {
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
	grayScale_->Init(device_, dxHeap_);
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
	// 画面に写るリソースとCSに送る定数バッファをコマンドに積む
	grayScale_->Draw(commandList);

	UINT groupCountX = (kWindowWidth_ + 16 - 1) / 16;
	UINT groupCountY = (kWindowHeight_ + 16 - 1) / 16;

	commandList->Dispatch(groupCountX, groupCountY, 1);
}

/// <summary>
/// UAVの状態を読み込みから書き込み状態にする
/// </summary>
/// <param name="commandList">コマンドリスト</param>
void ComputeShader::TransitionUAVResource(ID3D12GraphicsCommandList* commandList, const D3D12_RESOURCE_STATES& beforState, const D3D12_RESOURCE_STATES& afterState) {
	//TransitionResourceState(commandList, uavBuffer_.Get(), beforState, afterState);
	grayScale_->TransitionUAVResource(commandList, beforState, afterState);
}
