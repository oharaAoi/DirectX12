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
	materialBuffer_ = CreateBufferResource(device, sizeof(ResMaterial));
	materialBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	// 色を決める
	materialData_->color = Vector4(1.0f, 1.0f, 0.0f, 1.0f);
}

void Material::Draw(ID3D12GraphicsCommandList* commandList) {
	commandList->SetGraphicsRootConstantBufferView(0, materialBuffer_->GetGPUVirtualAddress());
}