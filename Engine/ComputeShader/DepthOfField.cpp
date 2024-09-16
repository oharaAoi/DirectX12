#include "DepthOfField.h"

DepthOfField::DepthOfField(const UINT& gpuGroupCountX, const UINT& gpuGroupCountY, ComputeShaderPipeline* pipeline) {
	groupCountX_ = gpuGroupCountX;
	groupCountY_ = gpuGroupCountY;
	pipeline_ = pipeline;
}

DepthOfField::~DepthOfField() {
}

void DepthOfField::Finalize() {
	BaseCSResource::Finalize();
	copyDepthStencilResource_.Reset();
}

void DepthOfField::Init(ID3D12Device* device, DescriptorHeap* dxHeap) {
	assert(device);
	assert(dxHeap);

	dxHeap_ = dxHeap;
	device_ = device;

	CreateResourceBuffer(1);
	// UAVのResource作成とViewの作成
	BaseCSResource::Init(device, dxHeap);

	depthStencilResource_ = DirectXCommon::GetInstacne()->GetDepthStencilResource();

	// cBufferの作成
	cBuffer_ = CreateBufferResource(device_, sizeof(DepthData));

	data_ = nullptr;
	cBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&data_));

	D3D12_RESOURCE_DESC textureDesc = {};
	textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureDesc.Width = kWindowWidth_;
	textureDesc.Height = kWindowHeight_;
	textureDesc.DepthOrArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	textureDesc.SampleDesc.Count = 1;
	//textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	// HEAPの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	device->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&textureDesc,
		D3D12_RESOURCE_STATE_COPY_DEST, // コピー先の状態
		nullptr,
		IID_PPV_ARGS(&copyDepthStencilResource_)
	);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	// SRVを作成するDescriptorHeapの場所を求める
	referenceResourceHandles_ = dxHeap_->GetDescriptorHandle(DescriptorHeapType::TYPE_SRV);
	// 生成
	device_->CreateShaderResourceView(copyDepthStencilResource_.Get(), &srvDesc, referenceResourceHandles_.handleCPU);

}

void DepthOfField::ConfigureResource(ID3D12GraphicsCommandList* commandList) {
	// 深度バッファからテクスチャへコピー
	D3D12_TEXTURE_COPY_LOCATION srcLocation = {};
	srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	srcLocation.pResource = depthStencilResource_;
	srcLocation.SubresourceIndex = 0; // 深度バッファのサブリソースインデックス

	D3D12_TEXTURE_COPY_LOCATION dstLocation = {};
	dstLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dstLocation.pResource = copyDepthStencilResource_.Get();
	dstLocation.SubresourceIndex = 0; // コピー先テクスチャのサブリソースインデックス

	// -------------------------------------------------
	// ↓ 
	// -------------------------------------------------
	TransitionResourceState(commandList, depthStencilResource_, D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_COPY_SOURCE);

	commandList->CopyTextureRegion(
		&dstLocation,
		0, 0, 0,   // コピー先の位置
		&srcLocation,
		nullptr   // 元のテクスチャの範囲
	);

	TransitionResourceState(commandList, copyDepthStencilResource_.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	
	// -------------------------------------------------
	// ↓ 
	// -------------------------------------------------
	pipeline_->SetPipelineState(commandList);
	BaseCSResource::ConfigureResource(commandList);
	commandList->Dispatch(groupCountX_, groupCountY_, 1);
	//TransitionUAVResource(commandList, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, 0);

	// -------------------------------------------------
	// ↓ 
	// -------------------------------------------------
	TransitionResourceState(commandList, depthStencilResource_, D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	TransitionResourceState(commandList, copyDepthStencilResource_.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_COPY_DEST);
}

void DepthOfField::TransitionResource(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES beforState, D3D12_RESOURCE_STATES afterState) {
	TransitionResourceState(commandList, bufferHandles_[0].uavBuffer.Get(), beforState, afterState);
}
