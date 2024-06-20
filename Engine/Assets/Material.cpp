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

	baseMaterial_->diffuseColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	baseMaterial_->specularColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	baseMaterial_->roughness = 0.5f;
	baseMaterial_->metallic = 0.5f;
	baseMaterial_->shininess = 50;
}

void Material::Update(const Matrix4x4& uvTransform) {
	baseMaterial_->uvTransform = uvTransform;
}

void Material::Draw(ID3D12GraphicsCommandList* commandList) {
	commandList->SetGraphicsRootConstantBufferView(0, materialBuffer_->GetGPUVirtualAddress());
}

void Material::ImGuiDraw() {
	//ImGui::DragFloat("Albedo", &baseMaterial_->color, 0.01f, 0.0f, 1.0f);
	ImGui::ColorEdit3("baseColor", &baseMaterial_->color.x);
	ImGui::ColorEdit3("diffuse", &baseMaterial_->diffuseColor.x);
	ImGui::ColorEdit3("specular", &baseMaterial_->specularColor.x);
	ImGui::DragFloat("roughness", &baseMaterial_->roughness, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("metallic", &baseMaterial_->metallic, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("shininess", &baseMaterial_->shininess, 01.0f, 1.0f, 100.0f);
}

void Material::SetMaterialData(MaterialData materialData) {
	materialData_ = materialData;

	baseMaterial_->color = materialData_.albedo;
	baseMaterial_->enableLighting = true;
	baseMaterial_->uvTransform = MakeIdentity4x4();
	// 反射用素
	baseMaterial_->diffuseColor = materialData_.diffuse;
	baseMaterial_->diffuseColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	baseMaterial_->specularColor = materialData_.specular;

	baseMaterial_->roughness = 0.5f;
	baseMaterial_->metallic = 0.5f;
	baseMaterial_->shininess = 50;
}

void Material::SetMaterialParameter(const float& roughness, const float& metallic) {
	baseMaterial_->roughness = roughness;
	baseMaterial_->metallic = metallic;
}
