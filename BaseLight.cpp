#include "BaseLight.h"

void BaseLight::Init(ID3D12Device* device, const size_t& size) {
	lightBuffer_ = CreateBufferResource(device, size);
}

void BaseLight::Finalize() {
	lightBuffer_.Reset();
}

void BaseLight::Update() {
}

void BaseLight::Draw(ID3D12GraphicsCommandList* commandList, const uint32_t& rootParameterIndex) {
	commandList->SetGraphicsRootConstantBufferView(rootParameterIndex, lightBuffer_->GetGPUVirtualAddress());
}
