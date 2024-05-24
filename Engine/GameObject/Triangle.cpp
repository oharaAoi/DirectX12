#include "Triangle.h"

Triangle::Triangle() {
}

Triangle::~Triangle() {
	mesh_->Finalize();
	material_->Finalize();
	transformation_->Finalize();
}

void Triangle::Init(ID3D12Device* device, const Mesh::Vertices& vertex) {
	mesh_ = std::make_unique<Mesh>();
	material_ = std::make_unique<Material>();
	transformation_ = std::make_unique<TransformationMatrix>();

	mesh_->Init(device, sizeof(Mesh::VertexData) * 3, 3);
	material_->Init(device);
	transformation_->Init(device);

	// vertexの設定
	Mesh::VertexData* vertexData = mesh_->GetVertexData();
	// 左下
	vertexData[0].pos = vertex.vertex1;
	vertexData[0].texcoord = { 0.0f, 1.0f };
	vertexData[0].normal = { 0.0f, 0.0f, -1.0f };
	// 上
	vertexData[1].pos = vertex.vertex2;
	vertexData[1].texcoord = { 0.5f, 0.0f };
	vertexData[1].normal = { 0.0f, 0.0f, -1.0f };
	// 右下
	vertexData[2].pos = vertex.vertex3;
	vertexData[2].texcoord = { 1.0f, 1.0f };
	vertexData[2].normal = { 0.0f, 0.0f, -1.0f };

	// indexの設定
	uint32_t* indexData = mesh_->GetIndexData();
	for (uint32_t index = 0; index < 3; index++) {
		indexData[index] = index;
	}

	// materialの設定
	Material::BaseMaterial* materialData = material_->GetMaterialData();
	materialData->enableLighting = false;
}

void Triangle::Update(const Matrix4x4& world, const Matrix4x4& view, const Matrix4x4& projection) {
	transformation_->Update(world, view, projection);
}

void Triangle::Draw(ID3D12GraphicsCommandList* commandList) {
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mesh_->Draw(commandList);
	material_->Draw(commandList);
	transformation_->Draw(commandList);
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, 0);
	//commandList->DrawInstanced(3, 1, 0, 0);
	commandList->DrawIndexedInstanced(3, 1, 0, 0, 0);
}
