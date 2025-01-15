#include "PointLight.h"

PointLight::PointLight() {
}

PointLight::~PointLight() {
}

void PointLight::Init(ID3D12Device* device, const size_t& size) {
	BaseLight::Init(device, size);
	lightBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData_));
	pointLightData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	pointLightData_->position = { 0.0f, 2.0f, 0.0f };
	pointLightData_->intensity = 1.0f;
	pointLightData_->radius = 10.0f;
	pointLightData_->decay = 1.0f;
}

void PointLight::Finalize() {
	BaseLight::Finalize();
}

void PointLight::Update() {
	BaseLight::Update();
}

void PointLight::Draw(ID3D12GraphicsCommandList* commandList, const uint32_t& rootParameterIndex) {
	BaseLight::Draw(commandList, rootParameterIndex);
}

#ifdef _DEBUG
void PointLight::Debug_Gui() {
	ImGui::DragFloat3("position", &pointLightData_->position.x, 0.1f, -10.0f, 10.0f);
	ImGui::DragFloat("intensity", &pointLightData_->intensity, 0.1f, 0.0f, 1.0f);
	ImGui::DragFloat("radius", &pointLightData_->radius, 0.1f, 0.0f, 10.0f);
	ImGui::DragFloat("decay", &pointLightData_->decay, 0.1f, 0.0f, 1.0f);
}
#endif
