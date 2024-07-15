#pragma once
// DirectX
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <memory>
#include <vector>
// PSO
#include "RootSignature.h"
#include "DirectXCompiler.h"
#include "DescriptorHeap.h"
#include "DescriptorSize.h"
#include "Shader.h"
//
#include "MyMath.h"
//
#include "TextureManager.h"

class ComputeShader {
public:

	ComputeShader();
	~ComputeShader();

	void Finalize();

	void Init(ID3D12Device* device, DirectXCompiler* dxCompiler,
			  DescriptorHeap* dxHeap, DescriptorSize* dxSize, 
			  ID3D12Resource* textureResource, const std::string& computeShaderPath);

	void SetPipelineState(ID3D12GraphicsCommandList* commandList);

	void SetUAVResource(ID3D12GraphicsCommandList* commandList);

	void CreateUAV();

	void Draw(ID3D12GraphicsCommandList* commandList);

private:
	// rootSignature
	std::unique_ptr<RootSignature> rootSignature_ = nullptr;
	// Shader
	ComPtr<IDxcBlob> computeShaderBlob_ = nullptr;
	// PSO
	ComPtr<ID3D12PipelineState> csPipelineState_ = nullptr;
	// 
	ComPtr<ID3D12Resource> uavBuffer_ = nullptr;
	
	// DXCで使う
	DirectXCompiler* dxCompiler_ = nullptr;
	// dxHeap
	DescriptorHeap* dxHeap_ = nullptr;
	// 
	DescriptorSize* dxSize_ = nullptr;

	// device
	ID3D12Device* device_ = nullptr;

	D3D12_CPU_DESCRIPTOR_HANDLE uavHandleCPU_;
	D3D12_GPU_DESCRIPTOR_HANDLE uavHandleGPU_;

	D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU_;
	D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU_;

	ID3D12Resource* textureResource_;

};

