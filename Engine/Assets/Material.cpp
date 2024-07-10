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
	materialBuffer_ = CreateBufferResource(device, sizeof(PBRMaterial));
	materialBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&pbrMaterial_));
	// 色を決める
	pbrMaterial_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	pbrMaterial_->enableLighting = true;
	pbrMaterial_->uvTransform = MakeIdentity4x4();
	pbrMaterial_->shininess = 50;

	pbrMaterial_->diffuseColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	pbrMaterial_->specularColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	pbrMaterial_->roughness = 0.5f;
	pbrMaterial_->metallic = 0.5f;
}

void Material::Update(const Matrix4x4& uvTransform) {
	pbrMaterial_->uvTransform = uvTransform;
}

void Material::Draw(ID3D12GraphicsCommandList* commandList) {
	commandList->SetGraphicsRootConstantBufferView(0, materialBuffer_->GetGPUVirtualAddress());
}

void Material::ImGuiDraw() {
	//ImGui::DragFloat("Albedo", &pbrMaterial_->color, 0.01f, 0.0f, 1.0f);
	ImGui::ColorEdit4("baseColor", &pbrMaterial_->color.x);
	ImGui::ColorEdit3("diffuse", &pbrMaterial_->diffuseColor.x);
	ImGui::ColorEdit3("specular", &pbrMaterial_->specularColor.x);
	ImGui::DragFloat("roughness", &pbrMaterial_->roughness, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("metallic", &pbrMaterial_->metallic, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("shininess", &pbrMaterial_->shininess, 01.0f, 1.0f, 100.0f);
}

void Material::SetMaterialData(ModelMaterialData materialData) {
	materialData_ = materialData;

	pbrMaterial_->color = materialData_.albedo;
	pbrMaterial_->enableLighting = true;
	pbrMaterial_->uvTransform = MakeIdentity4x4();
	// 反射用素
	pbrMaterial_->diffuseColor = materialData_.diffuse;
	pbrMaterial_->diffuseColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	pbrMaterial_->specularColor = materialData_.specular;

	pbrMaterial_->roughness = 0.5f;
	pbrMaterial_->metallic = 0.5f;
	pbrMaterial_->shininess = 50;

	pbrMaterial_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	pbrMaterial_->enableLighting = true;
	pbrMaterial_->uvTransform = MakeIdentity4x4();
	pbrMaterial_->shininess = 50;

	pbrMaterial_->diffuseColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	pbrMaterial_->specularColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	pbrMaterial_->roughness = 0.5f;
	pbrMaterial_->metallic = 0.5f;
}

void Material::SetMaterialParameter(const float& roughness, const float& metallic) {
	pbrMaterial_->roughness = roughness;
	pbrMaterial_->metallic = metallic;
}
