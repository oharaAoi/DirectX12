#include "RenderTexture.h"

RenderTexture::RenderTexture() {
}

RenderTexture::~RenderTexture() {
}

void RenderTexture::Finalize() {
	renderResource_.Reset();
	vertexBuffer_.Reset();
	indexBuffer_.Reset();
	materialBuffer_.Reset();
	transformBuffer_.Reset();
	DescriptorHeap::AddFreeSrvList(renderSrvRenderAddress_.assignIndex_);
	DescriptorHeap::AddFreeSrvList(renderUavRenderAddress_.assignIndex_);
}

void RenderTexture::Init(ID3D12Device* device, DescriptorHeap* dxHeap) {
	// ----------------------------------------------------------------------------------
	vertexBuffer_ = CreateBufferResource(device, sizeof(TextureMesh) * 4);
	// リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(TextureMesh) * 4;
	// 1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(TextureMesh);
	// Resourceにデータを書き込む 
	vertexData_ = nullptr;
	// アドレスを取得
	vertexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	vertexData_[0].pos = { 0, 720, 0,1 };
	vertexData_[0].texcoord = { 0.0f, 1.0f }; // 左下
	vertexData_[1].pos = { 0,0,0,1 };
	vertexData_[1].texcoord = { 0.0f, 0.0f }; // 左上
	vertexData_[2].pos = { 1280, 720, 0,1 }; // 右下
	vertexData_[2].texcoord = { 1.0f, 1.0f };
	vertexData_[3].pos = { 1280, 0, 0, 1 };		// 右上
	vertexData_[3].texcoord = { 1.0f, 0.0f };

	// ----------------------------------------------------------------------------------
	indexBuffer_ = CreateBufferResource(device, sizeof(uint32_t) * 6);
	indexBufferView_.BufferLocation = indexBuffer_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * 6);
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
	indexData_ = nullptr;
	indexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	indexData_[0] = 0;
	indexData_[1] = 1;
	indexData_[2] = 2;

	indexData_[3] = 1;
	indexData_[4] = 3;
	indexData_[5] = 2;
	// ----------------------------------------------------------------------------------
	materialBuffer_ = CreateBufferResource(device, sizeof(TextureMaterial));
	materialBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	materialData_->uvTransform = MakeIdentity4x4();
	materialData_->uvDrawRange = { 1.0f, 1.0f };
	
	// ----------------------------------------------------------------------------------
	transformBuffer_ = CreateBufferResource(device, sizeof(TextureTransformData));
	transformBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&transformData_));

	kTransform transform = { {1.0f,1.0f,1.0f} , {0.0f, 0.0f, 0.0f}, {0, 0, 0} };

	transformData_->wvp = Matrix4x4(
		MakeAffineMatrix(transform)
		* Inverse(MakeAffineMatrix(transform))
		* MakeOrthograhicMatrix(0.0f, 0.0f, float(1280), float(720), 0.0f, 100.0f)
	);

	// 最終的に描画させるResourceの作成
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

	// HEAPの設定
	// Resourceの作成
	renderResource_ = CerateShaderResource(device, &desc, &heapProperties, D3D12_HEAP_FLAG_ALLOW_DISPLAY, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	// ------------------------------------------------------------
	// UAVの設定
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Format = renderResource_->GetDesc().Format;
	// SRVを作成するDescriptorHeapの場所を求める
	renderUavRenderAddress_ = dxHeap->AllocateSRV();
	// 生成
	device->CreateUnorderedAccessView(renderResource_.Get(), nullptr, &uavDesc, renderUavRenderAddress_.handleCPU);

	// ------------------------------------------------------------
	// SRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	// SRVを作成するDescriptorHeapの場所を求める
	renderSrvRenderAddress_ = dxHeap->AllocateSRV();
	// 生成
	device->CreateShaderResourceView(renderResource_.Get(), &srvDesc, renderSrvRenderAddress_.handleCPU);
}

void RenderTexture::Draw(ID3D12GraphicsCommandList* commandList) {
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	commandList->IASetIndexBuffer(&indexBufferView_);
	commandList->SetGraphicsRootConstantBufferView(0, materialBuffer_->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(1, transformBuffer_->GetGPUVirtualAddress());
	commandList->SetGraphicsRootDescriptorTable(2, renderSrvRenderAddress_.handleGPU);
	commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void RenderTexture::TransitionResource(ID3D12GraphicsCommandList* commandList, const D3D12_RESOURCE_STATES& beforState, const D3D12_RESOURCE_STATES& afterState) {
	TransitionResourceState(commandList, renderResource_.Get(), beforState, afterState);
}
