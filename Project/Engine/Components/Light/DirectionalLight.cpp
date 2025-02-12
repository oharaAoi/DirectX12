#include "DirectionalLight.h"
#include "Engine/Lib/Json//JsonItems.h"

DirectionalLight::DirectionalLight() {
}

DirectionalLight::~DirectionalLight() {
}

void DirectionalLight::Init(ID3D12Device* device, const size_t& size) {
	AttributeGui::SetName("Directional Light");
	BaseLight::Init(device, size);
	lightBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));

	directionalLightData_->color = parameter_.color;
	directionalLightData_->direction = parameter_.direction;
	directionalLightData_->intensity = parameter_.intensity;
	directionalLightData_->limPower = parameter_.limPower;

}

void DirectionalLight::Finalize() {
	BaseLight::Finalize();
}

void DirectionalLight::Update() {
	directionalLightData_->direction = Normalize(directionalLightData_->direction);
	BaseLight::Update();
}

void DirectionalLight::Draw(ID3D12GraphicsCommandList* commandList, const uint32_t& rootParameterIndex) {
	BaseLight::Draw(commandList, rootParameterIndex);
}

#ifdef _DEBUG
void DirectionalLight::Debug_Gui() {
	ImGui::ColorEdit4("color", &parameter_.color.x);
	ImGui::DragFloat3("direction", &parameter_.direction.x, 0.1f, -1.0f, 1.0f);
	ImGui::DragFloat("intensity", &parameter_.intensity, 0.1f, 0.0f, 1.0f);
	ImGui::DragFloat("limPower", &parameter_.limPower, 0.1f, 0.0f, 10.0f);

	parameter_.direction = Normalize(parameter_.direction);

	directionalLightData_->color = parameter_.color;
	directionalLightData_->direction = parameter_.direction;
	directionalLightData_->intensity = parameter_.intensity;
	directionalLightData_->limPower = parameter_.limPower;

	if (ImGui::Button("Save")) {
		JsonItems::Save("Light", parameter_.ToJson("directionalLight"));
	}
	if (ImGui::Button("Apply")) {
		parameter_.FromJson(JsonItems::GetData("Light", "directionalLight"));
	}
}
#endif
