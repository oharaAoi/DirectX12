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
}

void Material::Update(const Matrix4x4& uvTransform) {
	baseMaterial_->uvTransform = uvTransform;
}

void Material::Draw(ID3D12GraphicsCommandList* commandList) {
	commandList->SetGraphicsRootConstantBufferView(0, materialBuffer_->GetGPUVirtualAddress());
}