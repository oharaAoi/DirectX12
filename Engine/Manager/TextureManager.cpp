#include "TextureManager.h"

TextureManager* TextureManager::GetInstance() {
	static TextureManager instance;
	return &instance;
}

void TextureManager::Init(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* srvHeap, const uint32_t& srvDescriptorSize) {
	assert(device);
	assert(srvHeap);

	device_ = device;
	srvHeap_ = srvHeap;
	srvDescriptorSize_ = srvDescriptorSize;

	srvData_.clear();

	CreateShaderResource("uvChecker.png", commandList);
	CreateShaderResource("monsterBall.png", commandList);
	CreateShaderResource("checkerBoard.png", commandList);
	CreateShaderResource("fence.png", commandList);
	CreateShaderResource("circle.png", commandList);
	CreateShaderResource("grass.png", commandList);
	CreateShaderResource("skin.png", commandList);
	
	//
	CreateShaderResource("Materials/brick/Brick_Cracked_001_1K_BaseColor.jpg", commandList);
	CreateShaderResource("Materials/brick/Brick_Cracked_001_1K_Normal.jpg", commandList);
	CreateShaderResource("Materials/brick/Brick_Cracked_001_1K_Metallic.jpg", commandList);
	CreateShaderResource("Materials/brick/Brick_Cracked_001_1K_Roughness.jpg", commandList);

	//
	CreateShaderResource("Materials/leather/LEATHER_BROWN_TENSE_SCRATCH_1K_BaseColor.jpg", commandList);
	CreateShaderResource("Materials/leather/LEATHER_BROWN_TENSE_SCRATCH_1K_Normal.jpg", commandList);
	CreateShaderResource("Materials/leather/LEATHER_BROWN_TENSE_SCRATCH_1K_Metallic.jpg", commandList);
	CreateShaderResource("Materials/leather/LEATHER_BROWN_TENSE_SCRATCH_1K_Roughness.jpg", commandList);

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
	CreateShaderResource("Materials/cloth/FABRIC_GREEN_001_1K_Roughness.jpg", commandList);

	CreateShaderResource("Materials/block/TERRAZZO_CRACKED_004_1K_BaseColor.jpg", commandList);
	CreateShaderResource("Materials/block/TERRAZZO_CRACKED_004_1K_Normal.jpg", commandList);
	CreateShaderResource("Materials/block/TERRAZZO_CRACKED_004_1K_Metallic.jpg", commandList);
	CreateShaderResource("Materials/block/TERRAZZO_CRACKED_004_1K_Roughness.jpg", commandList);*/

	//LoadWhite1x1Texture("white1x1.png", commandList);

}

void TextureManager::Finalize() {
	for (auto& data : srvData_) {
		data.second.textureResource_.Reset();
		data.second.intermediateResource_.Reset();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////////////////
void TextureManager::CreateShaderResource(const std::string& filePath, ID3D12GraphicsCommandList* commandList) {
	SRVData data{};

	DirectX::ScratchImage mipImage = LoadTexture("Resources/" + filePath);
	const DirectX::TexMetadata& metadata = mipImage.GetMetadata();
	data.textureResource_ = CreateTextureResource(device_, metadata);
	data.intermediateResource_ = UploadTextureData(data.textureResource_, mipImage, device_, commandList);

	// ------------------------------------------------------------
	// metadataを元にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	// ------------------------------------------------------------
	// SRVを作成するDescriptorHeapの場所を求める
	data.address_.handleCPU = GetCPUDescriptorHandle(srvHeap_, srvDescriptorSize_, (int(srvData_.size()) + 1));
	data.address_.handleGPU = GetGPUDescriptorHandle(srvHeap_, srvDescriptorSize_, (int(srvData_.size()) + 1));
	
	// 配列に入れる
	srvData_["Resources/" + filePath] = data;

	// 生成
	device_->CreateShaderResourceView(data.textureResource_.Get(), &srvDesc, data.address_.handleCPU);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Textrueデータを読む
/////////////////////////////////////////////////////////////////////////////////////////////
DirectX::ScratchImage TextureManager::LoadTexture(const std::string& filePath) {
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertWString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	// ミニマップの作成
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	return mipImages;
}

void TextureManager::LoadWhite1x1Texture(const std::string& filePath, ID3D12GraphicsCommandList* commandList) {
	SRVData data{};

	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertWString("Resources/" + filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	const DirectX::TexMetadata& metadata = image.GetMetadata();
	data.textureResource_ = CreateTextureResource(device_, metadata);
	data.intermediateResource_ = UploadTextureData(data.textureResource_, image, device_, commandList);

	// ------------------------------------------------------------
	// metadataを元にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	// ------------------------------------------------------------
	// SRVを作成するDescriptorHeapの場所を求める
	data.address_.handleCPU = GetCPUDescriptorHandle(srvHeap_, srvDescriptorSize_, (int(srvData_.size()) + 1));
	data.address_.handleGPU = GetGPUDescriptorHandle(srvHeap_, srvDescriptorSize_, (int(srvData_.size()) + 1));
	
	lastSrvHandleCPU_ =data.address_.handleCPU;
	lastSrvHandleGPU_ =data.address_.handleGPU;

	// mapに値を代入
	srvData_.emplace("Resources/" + filePath, data);

	// 生成
	device_->CreateShaderResourceView(data.textureResource_.Get(), &srvDesc, data.address_.handleCPU);
}

Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::CreateTextureResource(Microsoft::WRL::ComPtr<ID3D12Device> device, const DirectX::TexMetadata& metadata) {
	// metaDataを元にResourceを設定
	D3D12_RESOURCE_DESC desc{};
	desc.Width = UINT(metadata.width);								// Textureの幅
	desc.Height = UINT(metadata.height);							// Textureの高さ
	desc.MipLevels = UINT16(metadata.mipLevels);					// mipmapの数
	desc.DepthOrArraySize = UINT16(metadata.arraySize);				// 奥行き　or 配列Textureの配数
	desc.Format = metadata.format;									// TextureのFormat
	desc.SampleDesc.Count = 1;										// サンプリングカウント
	desc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);	// Textureの次元数

	// HEAPの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,					// Heapの設定
		D3D12_HEAP_FLAG_NONE,				// Heapの特殊の設定
		&desc,								// Resourceの設定
		D3D12_RESOURCE_STATE_COPY_DEST,	// 初回のResourceState Textureは木基本読むだけ
		nullptr,							// 初回のResourceState Textureは木基本読むだけ
		IID_PPV_ARGS(&resource)				// 作成するResourceポインタへのポインタ
	);
	assert(SUCCEEDED(hr));

	return resource;
}

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

void TextureManager::SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* commandList, const std::string& filePath) {
	commandList->SetGraphicsRootDescriptorTable(3, srvData_[filePath].address_.handleGPU);
}

/// <summary>
/// srvの作成
/// </summary>
/// <param name="resource"></param>
/// <param name="format"></param>
void TextureManager::CreateShaderResourceView(ID3D12Resource* resource, const DXGI_FORMAT& format) {
	SRVData data{};

	// ------------------------------------------------------------
	// metadataを元にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	
	// ------------------------------------------------------------
	// SRVを作成するDescriptorHeapの場所を求める
	data.address_.handleCPU = GetCPUDescriptorHandle(srvHeap_, srvDescriptorSize_, (int(srvData_.size()) + 1));
	data.address_.handleGPU = GetGPUDescriptorHandle(srvHeap_, srvDescriptorSize_, (int(srvData_.size()) + 1));

	// mapに値を代入
	srvData_.emplace("offScreenTexture",data);

	// 生成
	device_->CreateShaderResourceView(resource, &srvDesc, data.address_.handleCPU);
}

void TextureManager::SetRenderTexture(ID3D12GraphicsCommandList* commandList, const uint32_t& index) {
	commandList->SetGraphicsRootDescriptorTable(index, srvData_["offScreenTexture"].address_.handleGPU);
}

void TextureManager::SetCsRenderTexture(ID3D12GraphicsCommandList* commandList, const uint32_t& index) {
	commandList->SetComputeRootDescriptorTable(index, srvData_["offScreenTexture"].address_.handleGPU);
}
