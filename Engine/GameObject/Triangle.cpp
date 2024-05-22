#include "Triangle.h"

Triangle::Triangle() {
}

Triangle::~Triangle() {
}

void Triangle::Init(ID3D12Device* device, const Mesh::Vertices& vertex) {
	mesh_ = std::make_unique<Mesh>();
	material_ = std::make_unique<Material>();

	mesh_->Init(device, sizeof(Mesh::VertexData) * 3);
	material_->Init(device);

	// vertexの設定
	Mesh::VertexData* vertexData = mesh_->GetVertexData();
	// 左下
	vertexData[0].pos = vertex.vertex1;
	// 上
	vertexData[1].pos = vertex.vertex2;
	// 右下
	vertexData[2].pos = vertex.vertex3;
}

void Triangle::Draw(ID3D12GraphicsCommandList* commandList) {
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mesh_->Draw(commandList);
	material_->Draw(commandList);

	commandList->DrawInstanced(3, 1, 0, 0);
}
