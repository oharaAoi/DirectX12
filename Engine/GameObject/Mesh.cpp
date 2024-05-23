#include "Mesh.h"

Mesh::Mesh() {
}

Mesh::~Mesh() {
	Finalize();
}

void Mesh::Finalize() {
	vertexBuffer_.Reset();
}

void Mesh::Init(ID3D12Device* device, const uint32_t& vBSize, const uint32_t& iBSize) {
	// ---------------------------------------------------------------
	// ↓Vetrtexの設定
	// ---------------------------------------------------------------
	// VertexBufferViewを作成する 
	vertexBuffer_ = CreateBufferResource(device, vBSize);
	// リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = vBSize;
	// 1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
	// Resourceにデータを書き込む 
	vertexData_ = nullptr;
	// アドレスを取得
	vertexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	// ---------------------------------------------------------------
	// ↓indexの設定
	// ---------------------------------------------------------------
	indexBuffer_ = CreateBufferResource(device, sizeof(uint32_t) * iBSize);
	indexBufferView_.BufferLocation = indexBuffer_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * iBSize);
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	indexData_ = nullptr;
	indexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
	/*for (uint32_t oi = 0; oi < iBSize; oi++) {
		indexData_[oi] = oi;
	}*/
}

void Mesh::Draw(ID3D12GraphicsCommandList* commandList) {
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
	commandList->IASetIndexBuffer(&indexBufferView_);
	//commandList->IASetIndexBuffer(&indexBufferView_);
}
