#include "Mesh.h"

Mesh::Mesh() {
}

Mesh::~Mesh() {
	Finalize();
}

void Mesh::Finalize() {
	vertexBuffer_.Reset();
	indexBuffer_.Reset();
}

void Mesh::CalculateTangent(VertexData* data) {
	VertexData vertex1 = data[0];
	VertexData vertex2 = data[1];
	VertexData vertex3 = data[2];

	// エッジベクトルを計算する(頂点の位置座標の変化を表すベクトル)
	Vector4 edge1 = vertex2.pos - vertex1.pos;
	Vector4 edge2 = vertex3.pos - vertex1.pos;

	// UVの差分ベクトルを計算する
	Vector2 deltaUV1 = vertex2.texcoord - vertex1.texcoord;
	Vector2 deltaUV2 = vertex3.texcoord - vertex1.texcoord;

	// スケーリング係数
	float coefficient = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	Vector3 tangent;
	tangent.x = coefficient * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent.y = coefficient * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent.z = coefficient * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

	tangent = Normalize(tangent);

	data[0].tangent = tangent;
	data[1].tangent = tangent;
	data[2].tangent = tangent;
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