#include "Sprite.h"

Sprite::Sprite() {
}

Sprite::~Sprite() {
	mesh_->Finalize();
	material_->Finalize();
}

void Sprite::Init(ID3D12Device* device, const Mesh::RectVetices& Rect) {
	mesh_ = std::make_unique<Mesh>();
	material_ = std::make_unique<Material>();

	mesh_->Init(device, sizeof(Mesh::VertexData) * 6, 6);
	material_->Init(device);
	
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
	Material::PBRMaterial* materialData = material_->GetBaseMaterial();
	materialData->enableLighting = false;

	uvTransform_ = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };

	// ----------------------------------------------------
	transformBuffer_ = CreateBufferResource(device, sizeof(TextureTransformData));
	transformBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&transformData_));

	transform_ = { {1.0f,1.0f,1.0f} , {0.0f, 0.0f, 0.0f}, {0, 0, 0} };

	transformData_->wvp = Matrix4x4(
		MakeAffineMatrix(transform_)
		* MakeIdentity4x4()
		* MakeOrthograhicMatrix(0.0f, 0.0f, float(1280), float(720), 0.0f, 100.0f)
	);
}

void Sprite::Update() {
	ImGui::DragFloat2("translation", &transform_.translate.x, 2.0f);
	ImGui::DragFloat2("scale", &transform_.scale.x, 0.01f, -10.0f, 10.0f);
	ImGui::SliderAngle("rotation", &transform_.rotate.z);
	if (ImGui::TreeNode("material")) {
		material_->ImGuiDraw();
		ImGui::TreePop();
	}

	transformData_->wvp = Matrix4x4(
		MakeAffineMatrix(transform_)
		* MakeIdentity4x4()
		* MakeOrthograhicMatrix(0.0f, 0.0f, float(1280), float(720), 0.0f, 100.0f)
	);
}

void Sprite::Draw(ID3D12GraphicsCommandList* commandList) {
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mesh_->Draw(commandList);
	material_->Draw(commandList);
	commandList->SetGraphicsRootConstantBufferView(1, transformBuffer_->GetGPUVirtualAddress());
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList, "Resources/uvChecker.png", 2);
	commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}