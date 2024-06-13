#include "Light.h"

Light::Light() {
}

Light::~Light() {
}

void Light::Init(ID3D12Device* device) {
	lightBuffer_ = CreateBufferResource(device, sizeof(DirectionalLight));
	lightBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&DirectionalLightData_));
	DirectionalLightData_->color = { 1.0f,1.0f, 1.0f, 1.0f };
	DirectionalLightData_->direction = { 0.0f, 0.0f, -1.0f };
	DirectionalLightData_->intensity = 1.0f;
}

void Light::Finalize() {
	lightBuffer_.Reset();
}

void Light::Update() {
}

void Light::Draw(ID3D12GraphicsCommandList* commandList, const uint32_t& rootParameterIndex) {
	commandList->SetGraphicsRootConstantBufferView(rootParameterIndex, lightBuffer_->GetGPUVirtualAddress());
}

void Light::ImGuiDraw() {
	ImGui::Begin("Light");
	ImGui::DragFloat3("Direction", &DirectionalLightData_->direction.x, 0.1f);
	ImGui::End();
}
