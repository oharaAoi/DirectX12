#include "Mesh.h"
#include "Engine/Assets/Skinning.h"

Mesh::Mesh() {
}

Mesh::~Mesh() {
	Finalize();
}

void Mesh::Finalize() {
	vertexBuffer_.Reset();
	indexBuffer_.Reset();
}

void Mesh::Init(ID3D12Device* device, std::vector<VertexData> vertexData, std::vector<uint32_t>& indices) {
	// ---------------------------------------------------------------
	// ↓Vetrtexの設定
	// ---------------------------------------------------------------
	// VertexBufferViewを作成する
	vertexBuffer_ = CreateBufferResource(device, vertexData.size() * sizeof(Mesh::VertexData));
	// リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = (UINT)vertexData.size() * sizeof(Mesh::VertexData);
	// 1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
	// Resourceにデータを書き込む 
	vertexData_ = nullptr;
	// アドレスを取得
	vertexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	std::memcpy(vertexData_, vertexData.data(), sizeof(VertexData) * vertexData.size());
	vertexDataSize_ = static_cast<uint32_t>(vertexData.size());

	verticesData_ = vertexData;

	// ---------------------------------------------------------------
	// ↓indexの設定
	// ---------------------------------------------------------------
	indices_ = indices;

	indexBuffer_ = CreateBufferResource(device, sizeof(uint32_t) * indices.size());
	indexBufferView_.BufferLocation = indexBuffer_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * indices.size());
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	indexData_ = nullptr;
	indexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	std::memcpy(indexData_, indices.data(), sizeof(uint32_t) * indices.size());

	indexNum_ = (uint32_t)indices.size();
}

void Mesh::Draw(ID3D12GraphicsCommandList* commandList) {
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	commandList->IASetIndexBuffer(&indexBufferView_);
}

void Mesh::DrawIndex(ID3D12GraphicsCommandList* commandList) {
	commandList->IASetIndexBuffer(&indexBufferView_);
}

void Mesh::CopyVertexData(const VertexData* data) {
	std::memcpy(vertexData_, data, sizeof(VertexData) * vertexDataSize_);
}
