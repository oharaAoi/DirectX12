#include "Triangle.h"
#include "Engine/Engine.h"

Triangle::Triangle() {
}

Triangle::~Triangle() {
	mesh_->Finalize();
	material_->Finalize();
	worldTransform_->Finalize();
}

void Triangle::Init(ID3D12Device* device, const Mesh::Vertices& vertex, const std::string& textureName) {
	mesh_ = std::make_unique<Mesh>();
	material_ = std::make_unique<Material>();

	// vertexの設定
	std::vector<Mesh::VertexData> vertices;
	std::vector<uint32_t> indices;
	vertices.resize(3);
	indices.resize(3);

	// 左下
	vertices[0].pos = vertex.vertex1;
	vertices[0].texcoord = { 0.0f, 1.0f };
	vertices[0].normal = { 0.0f, 0.0f, -1.0f };
	// 上
	vertices[1].pos = vertex.vertex2;
	vertices[1].texcoord = { 0.5f, 0.0f };
	vertices[1].normal = { 0.0f, 0.0f, -1.0f };
	// 右下
	vertices[2].pos = vertex.vertex3;
	vertices[2].texcoord = { 1.0f, 1.0f };
	vertices[2].normal = { 0.0f, 0.0f, -1.0f };

	// indexの設定
	for (uint32_t index = 0; index < 3; index++) {
		indices[index] = index;
	}

	mesh_->Init(device, vertices, indices);
	material_->Init(device);

	worldTransform_ = Engine::CreateWorldTransform();

	textureName_ = textureName;
}

void Triangle::Update() {
	worldTransform_->Update();
}

void Triangle::Draw(ID3D12GraphicsCommandList* commandList, ViewProjection* viewProjection) const {
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mesh_->Draw(commandList);
	material_->Draw(commandList);
	worldTransform_->Draw(commandList);
	viewProjection->Draw(commandList);
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, textureName_, 3);
	commandList->DrawIndexedInstanced(3, 1, 0, 0, 0);
}

void Triangle::SetVertices(const Mesh::Vertices& vertex) {
	Mesh::VertexData* data = mesh_->GetVertexData();
	data[0].pos = vertex.vertex1; // 左下
	data[1].pos = vertex.vertex2; // 上
	data[2].pos = vertex.vertex3; // 右下
}

#ifdef _DEBUG
void Triangle::Debug_Gui() {
	worldTransform_->Debug_Gui();
	material_->ImGuiDraw();
}
#endif