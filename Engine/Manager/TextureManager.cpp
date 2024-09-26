#include "TextureManager.h"

std::map<std::string, TextureManager::SRVData> TextureManager::srvData_;
std::shared_ptr<DirectXDevice> TextureManager::device_ = nullptr;
std::shared_ptr<DescriptorHeap> TextureManager::dxHeap_ = nullptr;
ID3D12GraphicsCommandList* TextureManager::commandList_ = nullptr;

TextureManager* TextureManager::GetInstance() {
	static TextureManager instance;
	return &instance;
}

void TextureManager::Init(std::shared_ptr<DirectXDevice> device, ID3D12GraphicsCommandList* commandList, std::shared_ptr<DescriptorHeap> dxHeap) {
	assert(device);
	assert(dxHeap);

	device_ = device;
	dxHeap_ = dxHeap;

	srvData_.clear();

	commandList_ = commandList;

	//LoadTextureFile("Materials/brick/Brick_Cracked_001_1K_BaseColor.jpg");
	//LoadTextureFile("Materials/brick/Brick_Cracked_001_1K_Normal.jpg");
	//LoadTextureFile("Materials/brick/Brick_Cracked_001_1K_Metallic.jpg");
	//LoadTextureFile("Materials/brick/Brick_Cracked_001_1K_Roughness.jpg");

	////
	//LoadTextureFile("Materials/leather/LEATHER_BROWN_TENSE_SCRATCH_1K_BaseColor.jpg");
	//LoadTextureFile("Materials/leather/LEATHER_BROWN_TENSE_SCRATCH_1K_Normal.jpg");
	//LoadTextureFile("Materials/leather/LEATHER_BROWN_TENSE_SCRATCH_1K_Metallic.jpg");
	//LoadTextureFile("Materials/leather/LEATHER_BROWN_TENSE_SCRATCH_1K_Roughness.jpg");

	//LoadTextureFile("Materials/block/TERRAZZO_CRACKED_004_1K_BaseColor.jpg");
	//LoadTextureFile("Materials/block/TERRAZZO_CRACKED_004_1K_Normal.jpg");
	//LoadTextureFile("Materials/block/TERRAZZO_CRACKED_004_1K_Metallic.jpg");
	//LoadTextureFile("Materials/block/TERRAZZO_CRACKED_004_1K_Roughness.jpg");

	// 
	//CreateShaderResource("Materials/leather2/LEATHER_BROWN_SCRATCH_1K_BaseColor.jpg", commandList);
	//CreateShaderResource("Materials/leather2/LEATHER_BROWN_SCRATCH_1K_Normal.jpg", commandList);
	//CreateShaderResource("Materials/leather2/LEATHER_BROWN_SCRATCH_1K_Metallic.jpg", commandList);
	//CreateShaderResource("Materials/leather2/LEATHER_BROWN_SCRATCH_1K_Roughness.jpg", commandList);

	////
	//CreateShaderResource("Materials/wallBrick/WALL_BRICK_TILES_001_1K_BaseColor.jpg", commandList);
	//CreateShaderResource("Materials/wallBrick/WALL_BRICK_TILES_001_1K_Normal.jpg", commandList);
	//CreateShaderResource("Materials/wallBrick/WALL_BRICK_TILES_001_1K_Metallic.jpg", commandList);
	//CreateShaderResource("Materials/wallBrick/WALL_BRICK_TILES_001_1K_Roughness.jpg", commandList);

	//
	/*CreateShaderResource("Materials/metal/METAL_PANEL_014_1K_BaseColor.jpg", commandList);
	CreateShaderResource("Materials/metal/METAL_PANEL_014_1K_Normal.jpg", commandList);
	CreateShaderResource("Materials/metal/METAL_PANEL_014_1K_Metallic.jpg", commandList);
	CreateShaderResource("Materials/metal/METAL_PANEL_014_1K_Roughness.jpg", commandList);

	CreateShaderResource("Materials/cloth/FABRIC_GREEN_001_1K_BaseColor.jpg", commandList);
	CreateShaderResource("Materials/cloth/FABRIC_GREEN_001_1K_Normal.jpg", commandList);
	CreateShaderResource("Materials/cloth/FABRIC_GREEN_001_1K_Metallic.jpg", commandList);
	CreateShaderResource("Materials/cloth/FABRIC_GREEN_001_1K_Roughness.jpg", commandList);*/

	//LoadWhite1x1Texture("white1x1.png", commandList);

}

void TextureManager::Finalize() {
	for (auto& data : srvData_) {
		data.second.textureResource_.Reset();
		data.second.intermediateResource_.Reset();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Textureを読み込む
/////////////////////////////////////////////////////////////////////////////////////////////
void TextureManager::LoadTextureFile(const std::string& directoryPath, const std::string& filePath) {

	// 一度読み込んだファイルか確認する
	auto it = srvData_.find(filePath);
	if (it != srvData_.end()) {
		return;
	}

	Log("Begin Load Texture\n");
	Log("Texture  [" + filePath + "]\n");
	SRVData data{};

	DirectX::ScratchImage mipImage = LoadTexture(directoryPath, filePath);
	const DirectX::TexMetadata& metadata = mipImage.GetMetadata();

	// resourceDescの作成
	D3D12_RESOURCE_DESC desc = CreateResourceDesc(metadata);

	// HEAPの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	// shaderResourceの作成
	data.textureResource_ = CerateShaderResource(device_->GetDevice(), &desc, &heapProperties, D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_STATE_COPY_DEST);
	data.intermediateResource_ = UploadTextureData(data.textureResource_, mipImage, device_->GetDevice(), commandList_);

	// ------------------------------------------------------------
	// metadataを元にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	// ------------------------------------------------------------
	// SRVを作成するDescriptorHeapの場所を求める
	data.address_ = dxHeap_->GetDescriptorHandle(DescriptorHeapType::TYPE_SRV);
	
	// 配列に入れる
	srvData_[filePath] = data;

	// 生成
	device_->GetDevice()->CreateShaderResourceView(data.textureResource_.Get(), &srvDesc, data.address_.handleCPU);

	Log("Load Finish  [" + filePath + "]\n");
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Textrueデータを読む
/////////////////////////////////////////////////////////////////////////////////////////////
DirectX::ScratchImage TextureManager::LoadTexture(const std::string& directoryPath, const std::string& filePath) {
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertWString(directoryPath + filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	// ミニマップの作成
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	return mipImages;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//	White1x1のTextureを読み込む
/////////////////////////////////////////////////////////////////////////////////////////////
void TextureManager::LoadWhite1x1Texture(const std::string& directoryPath, const std::string& filePath, ID3D12GraphicsCommandList* commandList) {
	SRVData data{};

	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertWString(directoryPath + filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	const DirectX::TexMetadata& metadata = image.GetMetadata();

	// resourceDescの作成
	D3D12_RESOURCE_DESC desc = CreateResourceDesc(metadata);

	// HEAPの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	// shaderResourceの作成
	data.textureResource_ = CerateShaderResource(device_->GetDevice(), &desc, &heapProperties, D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_STATE_COPY_DEST);
	data.intermediateResource_ = UploadTextureData(data.textureResource_, image, device_->GetDevice(), commandList);

	// ------------------------------------------------------------
	// metadataを元にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	// ------------------------------------------------------------
	// SRVを作成するDescriptorHeapの場所を求める
	data.address_ = dxHeap_->GetDescriptorHandle(DescriptorHeapType::TYPE_SRV);

	// mapに値を代入
	srvData_.emplace(filePath, data);

	// 生成
	device_->GetDevice()->CreateShaderResourceView(data.textureResource_.Get(), &srvDesc, data.address_.handleCPU);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// TextureResourceにデータを転送する
/////////////////////////////////////////////////////////////////////////////////////////////
[[nodiscard]]
ComPtr<ID3D12Resource> TextureManager::UploadTextureData(ComPtr<ID3D12Resource> texture,
														const DirectX::ScratchImage& mipImage,
														 ComPtr<ID3D12Device> device,
														 ComPtr<ID3D12GraphicsCommandList> commandList) {
	std::vector<D3D12_SUBRESOURCE_DATA> subresources;
	DirectX::PrepareUpload(device.Get(), mipImage.GetImages(), mipImage.GetImageCount(), mipImage.GetMetadata(), subresources); // subresourceの生成
	uint64_t intermediateSize = GetRequiredIntermediateSize(texture.Get(), 0, UINT(subresources.size()));						// 必要なサイズを求める
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource = CreateBufferResource(device, intermediateSize);

	// データ転送をコマンドに積む
	UpdateSubresources(commandList.Get(), texture.Get(), intermediateResource.Get(), 0, 0, UINT(subresources.size()), subresources.data());

	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture.Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	commandList->ResourceBarrier(1, &barrier);

	return intermediateResource;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// ResourceDescを作成する
/////////////////////////////////////////////////////////////////////////////////////////////
D3D12_RESOURCE_DESC TextureManager::CreateResourceDesc(const DirectX::TexMetadata& metadata) {
	// metaDataを元にResourceを設定
	D3D12_RESOURCE_DESC desc{};
	desc.Width = UINT(metadata.width);								// Textureの幅
	desc.Height = UINT(metadata.height);							// Textureの高さ
	desc.MipLevels = UINT16(metadata.mipLevels);					// mipmapの数
	desc.DepthOrArraySize = UINT16(metadata.arraySize);				// 奥行き　or 配列Textureの配数
	desc.Format = metadata.format;									// TextureのFormat
	desc.SampleDesc.Count = 1;										// サンプリングカウント
	desc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);	// Textureの次元数

	return desc;
}

void TextureManager::SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* commandList, const std::string& filePath, const uint32_t& rootParameterIndex) {
	//commandList->SetGraphicsRootDescriptorTable(3, srvData_[filePath].address_.handleGPU);
	commandList->SetGraphicsRootDescriptorTable(rootParameterIndex, srvData_[filePath].address_.handleGPU);
}

