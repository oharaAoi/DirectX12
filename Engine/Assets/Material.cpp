#include "Material.h"

Material::Material() {
}

Material::~Material() {
	Finalize();
}

void Material::Finalize() {
	materialBuffer_.Reset();
}

void Material::Init(ID3D12Device* device) {
	// ---------------------------------------------------------------
	// ↓Materialの設定
	// ---------------------------------------------------------------
	materialBuffer_ = CreateBufferResource(device, sizeof(BaseMaterial));
	materialBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&baseMaterial_));
	// 色を決める
	baseMaterial_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	baseMaterial_->enableLighting = true;
	baseMaterial_->uvTransform = MakeIdentity4x4();
	baseMaterial_->aldedo = 1.0f;
	baseMaterial_->refractiveIndex = 1.5f;
	baseMaterial_->refraction = 0.5f;
}

void Material::Update(const Matrix4x4& uvTransform) {
	baseMaterial_->uvTransform = uvTransform;
}

void Material::Draw(ID3D12GraphicsCommandList* commandList) {
	commandList->SetGraphicsRootConstantBufferView(0, materialBuffer_->GetGPUVirtualAddress());
}

void Material::ImGuiDraw() {
	ImGui::DragFloat("Albedo", &baseMaterial_->aldedo, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("refractiveIndex", &baseMaterial_->refractiveIndex, 0.01f, 1.0f, 2.0f);
	ImGui::DragFloat("refraction", &baseMaterial_->refraction, 0.01f, 0.0f, 1.0f);
}