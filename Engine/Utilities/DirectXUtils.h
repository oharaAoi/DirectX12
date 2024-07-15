#pragma once
// DirectX
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <dxgidebug.h>

// dxc
#include <dxcapi.h>
#include <vector>

#include "Utilities/Convert.h"

#include <wrl.h>

/// <summary>
/// DiscriptorHeapの作成
/// </summary>
/// <param name="device"></param>
/// <param name="heapType"></param>
/// <param name="numDescriptor"></param>
/// <param name="shaderVisible"></param>
/// <returns></returns>
/// template<typename T>
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(Microsoft::WRL::ComPtr<ID3D12Device> device,
	const D3D12_DESCRIPTOR_HEAP_TYPE& heapType,
	const UINT& numDescriptor,
	const bool& shaderVisible);

/// <summary>
/// 
/// </summary>
/// <param name="device"></param>
/// <param name="sizeInBytes"></param>
/// <returns></returns>
Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(Microsoft::WRL::ComPtr<ID3D12Device> device, const size_t& sizeInBytes);

/// <summary>
/// 深度情報を格納するリソースの生成
/// </summary>
/// <param name="device"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <returns></returns>
Microsoft::WRL::ComPtr<ID3D12Resource> CreateDepthStencilTextureResource(Microsoft::WRL::ComPtr<ID3D12Device> device, const int32_t& width, const int32_t& height);

/// <summary>
/// CompileShader
/// </summary>
/// <param name=""></param>
Microsoft::WRL::ComPtr<IDxcBlob> CompilerShader(
	const std::wstring& filePath,
	const wchar_t* entryPoint,
	const wchar_t* profile,
	Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils,
	Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler,
	Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler
);

/// <summary>
/// リソースのstateを変更する関数
/// </summary>
/// <param name="commandList">コマンドリスト</param>
/// <param name="resource">リソース</param>
/// <param name="beforState">遷移前状態</param>
/// <param name="afterState">遷移後状態</param>
void TransitionResourceState(ID3D12GraphicsCommandList* commandList, ID3D12Resource* resource, D3D12_RESOURCE_STATES beforState, D3D12_RESOURCE_STATES afterState);



D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);

D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);

/// <summary>
/// ログを出す
/// </summary>
/// <param name="message"></param>
void Log(const std::string& message);