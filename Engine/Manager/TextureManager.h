#pragma once
#include <d3d12.h>
#include <cassert>
#include <vector>
#include <map>
#include <Externals/DirectXTex/DirectXTex.h>
#include <Externals/DirectXTex/d3dx12.h>

#include "Engine/Utilities/Convert.h"
#include "Engine/Utilities/DirectXUtils.h"
#include "Engine/DirectX/Descriptor/DescriptorHeap.h"
#include "Engine/DirectX/DirectXDevice/DirectXDevice.h"

template<typename T>
using ComPtr = Microsoft::WRL::ComPtr <T>;

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
	void Init(std::shared_ptr<DirectXDevice> device, ID3D12GraphicsCommandList* commandList, std::shared_ptr<DescriptorHeap> dxHeap);

	void Finalize();

public:

	static void LoadTextureFile(const std::string& directoryPath, const std::string& filePath);

	/// <summary>
	/// Textrueデータを読む
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	static DirectX::ScratchImage LoadTexture(const std::string& directoryPath, const std::string& filePath);

	static void LoadWhite1x1Texture(const std::string& directoryPath, const std::string& filePath, ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// TextureResourceにデータを転送する
	/// </summary>
	/// <param name="texture"></param>
	/// <param name="mipImage"></param>
	/// <param name="device"></param>
	/// <param name="commandList"></param>
	/// <returns></returns>
	[[nodiscard]]
	static ComPtr<ID3D12Resource> UploadTextureData(
		Microsoft::WRL::ComPtr<ID3D12Resource> texture,
		const DirectX::ScratchImage& mipImage,
		Microsoft::WRL::ComPtr<ID3D12Device> device,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList);

	/// <summary>
	/// textureをコマンドに積む
	/// </summary>
	/// <param name="commandList"></param>
	/// <param name="textureNum"></param>
	void SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* commandList, const std::string& filePath, const uint32_t& rootParameterIndex);

	/// <summary>
	/// ResourceDescを作成する
	/// </summary>
	/// <param name="metadata"></param>
	/// <returns></returns>
	static D3D12_RESOURCE_DESC CreateResourceDesc(const DirectX::TexMetadata& metadata);

	uint32_t GetSRVDataIndex() { return static_cast<uint32_t>(srvData_.size()); }

private:

	struct SRVData {
		ComPtr<ID3D12Resource> textureResource_ = nullptr;
		ComPtr<ID3D12Resource> intermediateResource_ = nullptr;

		DescriptorHeap::DescriptorHandles address_;
	};

	//std::vector<SRVData> srvData_;
	static std::map<std::string, SRVData> srvData_;

	// 生成で使う変数
	static std::shared_ptr<DirectXDevice> device_;
	static std::shared_ptr<DescriptorHeap> dxHeap_;
	static ID3D12GraphicsCommandList* commandList_;
};