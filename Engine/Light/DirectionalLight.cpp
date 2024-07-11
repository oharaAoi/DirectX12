#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() {
}

DirectionalLight::~DirectionalLight() {
}

void DirectionalLight::Init(ID3D12Device* device, const size_t& size) {
	BaseLight::Init(device, size);
	lightBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	directionalLightData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	directionalLightData_->direction = { 0.0f, 0.0f, -1.0f };
	directionalLightData_->intensity = 1.0f;
}

void DirectionalLight::Finalize() {
	BaseLight::Finalize();
}

void DirectionalLight::Update() {
	BaseLight::Update();
}

void DirectionalLight::Draw(ID3D12GraphicsCommandList* commandList, const uint32_t& rootParameterIndex) {
	directionalLightData_->direction = Normalize(directionalLightData_->direction);
	BaseLight::Draw(commandList, rootParameterIndex);
}

void DirectionalLight::ImGuiDraw() {
	ImGui::Begin("Light");
	ImGui::DragFloat3("Direction", &directionalLightData_->direction.x, 0.1f, -1.0f, 1.0f);
	ImGui::DragFloat("intensity", &directionalLightData_->intensity, 0.1f, 0.0f, 1.0f);
	
	ImGui::End();
}
