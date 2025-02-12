#include "PointLight.h"
#include "Engine/Lib/Json/JsonItems.h"


PointLight::PointLight() {
}

PointLight::~PointLight() {
}

void PointLight::Init(ID3D12Device* device, const size_t& size) {
	AttributeGui::SetName("Point Light");
	BaseLight::Init(device, size);
	lightBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData_));

	pointLightData_->color = parameter_.color;
	pointLightData_->position = parameter_.position;
	pointLightData_->intensity = parameter_.intensity;
	pointLightData_->radius = parameter_.radius;
	pointLightData_->decay = parameter_.decay;
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
	ImGui::ColorEdit4("color", &parameter_.color.x);
	ImGui::DragFloat3("position", &parameter_.position.x, 0.1f);
	ImGui::DragFloat("intensity", &parameter_.intensity, 0.1f, 0.0f, 1.0f);
	ImGui::DragFloat("radius", &parameter_.radius, 0.1f, 0.0f, 10.0f);
	ImGui::DragFloat("decay", &parameter_.decay, 0.1f, 0.0f, 1.0f);

	pointLightData_->color = parameter_.color;
	pointLightData_->position = parameter_.position;
	pointLightData_->intensity = parameter_.intensity;
	pointLightData_->radius = parameter_.radius;
	pointLightData_->decay = parameter_.decay;

	if (ImGui::Button("Save")) {
		JsonItems::Save("Light", parameter_.ToJson("pointLight"));
	}
	if (ImGui::Button("Apply")) {
		parameter_.FromJson(JsonItems::GetData("Light", "pointLight"));
	}
}
#endif
