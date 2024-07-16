#pragma once
// DirectX
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <memory>
#include <vector>
// PSO
#include "CSPipeline.h"
//
#include "MyMath.h"
//
#include "TextureManager.h"

class ComputeShader {
public:

	ComputeShader();
	~ComputeShader();

	void Finalize();

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="dxCompiler">コンパイラー</param>
	/// <param name="dxHeap">descriptorHeap</param>
	/// <param name="computeShaderPath">シェーダーのパス</param>
	void Init(ID3D12Device* device, DirectXCompiler* dxCompiler,
			  DescriptorHeap* dxHeap, DescriptorHeap::DescriptorHandles offScreenResourceAddress, 
			  const std::string& computeShaderPath);

	/// <summary>
	/// UAVを作成する
	/// </summary>
	void CreateUAV();

	/// <summary>
	/// パイプラインを設定する
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	void SetPipelineState(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// UAVの状態を読み込みから書き込み状態にする
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	void TransitionUAVResource(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 読み込み状態にしたUAVリソースをリストに積む
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* commandList);

private:
	// computeShader用のパイプライン
	std::unique_ptr<CSPipeline> computeShaderPipeline_ = nullptr;
	// uavResource
	ComPtr<ID3D12Resource> uavBuffer_ = nullptr;
	
	// ---------------------------------------
	// DXCで使う
	DirectXCompiler* dxCompiler_ = nullptr;
	// dxHeap
	DescriptorHeap* dxHeap_ = nullptr;
	// device
	ID3D12Device* device_ = nullptr;

	// ---------------------------------------
	DescriptorHeap::DescriptorHandles uavAddress_;
	DescriptorHeap::DescriptorHandles srvAddress_;
	DescriptorHeap::DescriptorHandles offScreenResourceAddress_;
};

