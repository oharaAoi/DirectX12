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
	pbrMaterial_->enableLighting = false;
	pbrMaterial_->uvTransform = MakeIdentity4x4();
	pbrMaterial_->shininess = 50;

	pbrMaterial_->diffuseColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	pbrMaterial_->specularColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	pbrMaterial_->roughness = 0.5f;
	pbrMaterial_->metallic = 0.5f;

	uvTranslation_ = { 0,0,0 };
	uvScale_ = { 1,1,1 };
	uvRotation_ = { 0,0,0 };
}

void Material::Update(const Matrix4x4& uvTransform) {
	pbrMaterial_->uvTransform = uvTransform;
}

void Material::Draw(ID3D12GraphicsCommandList* commandList) {
	commandList->SetGraphicsRootConstantBufferView(0, materialBuffer_->GetGPUVirtualAddress());
}

void Material::ImGuiDraw() {
	ImGui::DragFloat2("uvTranslate", &uvTranslation_.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat2("uvScale", &uvScale_.x, 0.01f, -10.0f, 10.0f);
	ImGui::SliderAngle("UvRotate", &uvRotation_.z);
	ImGui::ColorEdit3("color", &pbrMaterial_->color.x);
	ImGui::Combo("Lighting", &pbrMaterial_->enableLighting, "None\0Lambert\0HalfLambert");

	pbrMaterial_->uvTransform = MakeAffineMatrix(kTransform(uvScale_, uvRotation_, uvTranslation_));
}

void Material::SetMaterialData(ModelMaterialData materialData) {
	materialData_ = materialData;

	pbrMaterial_->color = materialData_.albedo;
	pbrMaterial_->enableLighting = true;
	pbrMaterial_->uvTransform = MakeIdentity4x4();
	// 反射用素
	pbrMaterial_->diffuseColor = materialData_.diffuse;
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
