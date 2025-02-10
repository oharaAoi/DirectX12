#include "Material.h"

Material::Material() {
}

Material::~Material() {
	Finalize();
}

void Material::Finalize() {
	materialBuffer_.Reset();
	material_ = nullptr;
}

void Material::Init(ID3D12Device* device, const Model::ModelMaterialData& materialData) {
	// ---------------------------------------------------------------
	// ↓Materialの設定
	// ---------------------------------------------------------------
	materialBuffer_ = CreateBufferResource(device, sizeof(MaterialData));
	materialBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&material_));
	// 色を決める
	SetMaterialData(materialData);

	uvTranslation_ = { 0,0,0 };
	uvScale_ = { 1,1,1 };
	uvRotation_ = { 0,0,0 };
}

void Material::Update(const Matrix4x4& uvTransform) {
	material_->uvTransform = uvTransform;
}

void Material::Draw(ID3D12GraphicsCommandList* commandList) {
	commandList->SetGraphicsRootConstantBufferView(0, materialBuffer_->GetGPUVirtualAddress());
}

#ifdef _DEBUG
void Material::ImGuiDraw() {
	if (ImGui::TreeNode("uvTramsform")) {
		if (ImGui::TreeNode("scale")) {
			ImGui::DragFloat4("uvScale", &uvScale_.x, 0.01f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("rotation")) {
			ImGui::DragFloat4("uvRotation", &uvRotation_.x, 0.01f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("translation")) {
			ImGui::DragFloat4("uvTranslation", &uvTranslation_.x, 0.01f);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	ImGui::ColorEdit4("color", &material_->color.x);
	ImGui::Combo("Lighting", &material_->enableLighting, "None\0Lambert\0HalfLambert");

	material_->uvTransform = SRT(uvScale_, uvRotation_, uvTranslation_).MakeAffine();
}
#endif

void Material::SetUseTexture(const std::string& name) {
	materialsData_.textureFilePath = name;
}

void Material::SetMaterialData(Model::ModelMaterialData materialData) {
	materialsData_ = materialData;
	
	material_->color = materialData.color;
	material_->enableLighting = true;
	material_->uvTransform = Matrix4x4::MakeUnit();
	material_->shininess = 50;
	materialsData_.textureFilePath = materialData.textureFilePath;
}