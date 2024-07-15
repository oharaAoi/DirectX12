#pragma once
#include <d3d12.h>
#include <cassert>
#include <vector>
#include <map>
#include <Externals/DirectXTex/DirectXTex.h>
#include <Externals/DirectXTex/d3dx12.h>

#include "Convert.h"
#include "DirectXUtils.h"

template<typename T>
using Comptr = Microsoft::WRL::ComPtr <T>;

class TextureManager {
public: // メンバ関数

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static TextureManager* GetInstance();

public:

	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(const TextureManager&) = delete;
	const TextureManager& operator=(const TextureManager&) = delete;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device"></param>
	void Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* srvHeap, const uint32_t& srvDescriptorSize);

	void Finalize();

public:

	void CreateShaderResource(const std::string& filePath, ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// Textrueデータを読む
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	DirectX::ScratchImage LoadTexture(const std::string& filePath);

	void LoadWhite1x1Texture(const std::string& filePath, ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// TextureResourceを作る
	/// </summary>
	/// <param name="device"></param>
	/// <param name="metadata"></param>
	/// <returns></returns>
	Comptr<ID3D12Resource> CreateTextureResource(Microsoft::WRL::ComPtr<ID3D12Device> device, const DirectX::TexMetadata& metadata);

	/// <summary>
	/// TextureResourceにデータを転送する
	/// </summary>
	/// <param name="texture"></param>
	/// <param name="mipImage"></param>
	/// <param name="device"></param>
	/// <param name="commandList"></param>
	/// <returns></returns>
	[[nodiscard]]
	Comptr<ID3D12Resource> UploadTextureData(
		Microsoft::WRL::ComPtr<ID3D12Resource> texture,
		const DirectX::ScratchImage& mipImage,
		Microsoft::WRL::ComPtr<ID3D12Device> device,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList);

	/// <summary>
	/// textureをコマンドに積む
	/// </summary>
	/// <param name="commandList"></param>
	/// <param name="textureNum"></param>
	void SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* commandList, const std::string& filePath);

	/// <summary>
	/// srvの作成
	/// </summary>
	/// <param name="resource"></param>
	/// <param name="format"></param>
	void CreateShaderResourceView(ID3D12Resource* resource, const DXGI_FORMAT& format);

	uint32_t GetSRVDataIndex() { return static_cast<uint32_t>(srvData_.size()); }

	void SetRenderTexture(ID3D12GraphicsCommandList* commandList, const uint32_t& index);

private:

	struct SRVData {
		Comptr<ID3D12Resource> textureResource_ = nullptr;
		Comptr<ID3D12Resource> intermediateResource_ = nullptr;

		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU_;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU_;
	};

	//std::vector<SRVData> srvData_;
	std::map<std::string, SRVData> srvData_;

	D3D12_CPU_DESCRIPTOR_HANDLE lastSrvHandleCPU_;
	D3D12_GPU_DESCRIPTOR_HANDLE lastSrvHandleGPU_;

	// 生成で使う変数
	ID3D12Device* device_ = nullptr;
	ID3D12DescriptorHeap* srvHeap_ = nullptr;
	uint32_t srvDescriptorSize_;
};