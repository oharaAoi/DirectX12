#include "PBRMaterial.h"

PBRMaterial::PBRMaterial() {
}

PBRMaterial::~PBRMaterial() {
}

void PBRMaterial::Finalize() {
}

void PBRMaterial::Init(ID3D12Device* device) {
	materialBuffer_ = CreateBufferResource(device, sizeof(PBRMaterialData));
	materialBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&pbrMaterial_));

	pbrMaterial_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	pbrMaterial_->enableLighting = false;
	pbrMaterial_->uvTransform = MakeIdentity4x4();
	pbrMaterial_->shininess = 50;

	pbrMaterial_->diffuseColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	pbrMaterial_->specularColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	pbrMaterial_->roughness = 0.5f;
	pbrMaterial_->metallic = 0.5f;
}

void PBRMaterial::Update() {
	pbrMaterial_->uvTransform = MakeAffineMatrix(uvScale_, uvRotation_, uvTranslation_);
}

void PBRMaterial::Draw(ID3D12GraphicsCommandList* commandList) const {
	commandList->SetGraphicsRootConstantBufferView(0, materialBuffer_->GetGPUVirtualAddress());
}
