#include "Sprite.h"

Sprite::Sprite() {
}

Sprite::~Sprite() {
	mesh_->Finalize();
	material_->Finalize();
	transformation_->Finalize();
}

void Sprite::Init(ID3D12Device* device, const Mesh::RectVetices& Rect) {
	mesh_ = std::make_unique<Mesh>();
	material_ = std::make_unique<Material>();
	transformation_ = std::make_unique<TransformationMatrix>();

	mesh_->Init(device, sizeof(Mesh::VertexData) * 6, 6);
	material_->Init(device);
	transformation_->Init(device, 1);

	// vertexの設定
	Mesh::VertexData* vertexData = mesh_->GetVertexData();
	vertexData[0].pos = Rect.leftBottom;
	vertexData[0].texcoord = { 0.0f, 1.0f };
	vertexData[0].normal = { 0.0f, 0.0f, -1.0f };
	vertexData[1].pos = Rect.leftTop;
	vertexData[1].texcoord = { 0.0f, 0.0f };
	vertexData[1].normal = { 0.0f, 0.0f, -1.0f };
	vertexData[2].pos = Rect.rightBottom;
	vertexData[2].texcoord = { 1.0f, 1.0f };
	vertexData[2].normal = { 0.0f, 0.0f, -1.0f };
	vertexData[3].pos = Rect.rightTop;		// 右上
	vertexData[3].texcoord = { 1.0f, 0.0f };
	vertexData[3].normal = { 0.0f, 0.0f, -1.0f };
	
	// indexの設定
	uint32_t* indexData = mesh_->GetIndexData();
	indexData[0] = 0;
	indexData[1] = 1;
	indexData[2] = 2;

	indexData[3] = 1;
	indexData[4] = 3;
	indexData[5] = 2;

	// materialの設定
	Material::BaseMaterial* materialData = material_->GetBaseMaterial();
	materialData->enableLighting = false;

	uvTransform_ = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };

	//TransformationMatrix::ResTransformationMatrix* data = transformation_->GetTransformationData();
}

void Sprite::Update(const Matrix4x4& world, const Matrix4x4& view, const Matrix4x4& projection) {
	material_->Update(MakeAffineMatrix(uvTransform_));
	transformation_->Update(world, view, projection);

	ImGui::Begin("Setting");
	ImGui::DragFloat2("uvTranslate", &uvTransform_.translate.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat2("uvScale", &uvTransform_.scale.x, 0.01f, -10.0f, 10.0f);
	ImGui::SliderAngle("UvRotate", &uvTransform_.rotate.z);
	ImGui::End();
}

void Sprite::Draw(ID3D12GraphicsCommandList* commandList) {
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mesh_->Draw(commandList);
	material_->Draw(commandList);
	transformation_->Draw(commandList);
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, "Resources/uvChecker.png");
	commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}
