#include "SpotLight.h"
#include "Engine/Utilities/AdjustmentItem.h"

SpotLight::SpotLight() {
}

SpotLight::~SpotLight() {
}

void SpotLight::Init(ID3D12Device* device, const size_t& size) {
	BaseLight::Init(device, size);
	lightBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&spotLightData_));

	spotLightData_->color = parameter_.color;
	spotLightData_->position = parameter_.position;
	spotLightData_->intensity = parameter_.intensity;
	spotLightData_->direction = parameter_.direction;
	spotLightData_->distance = parameter_.distance;
	spotLightData_->decay = parameter_.decay;
	spotLightData_->cosAngle = parameter_.cosAngle;
	spotLightData_->cosFalloffStart = parameter_.cosFalloffStart;

	cosDegree_ = std::cos(std::numbers::pi_v<float> / 3.0f);
	falloffDegree_ = std::cos(std::numbers::pi_v<float> / 4.0f);
}

void SpotLight::Finalize() {
	BaseLight::Finalize();
}

void SpotLight::Update() {
	BaseLight::Update();
}

void SpotLight::Draw(ID3D12GraphicsCommandList* commandList, const uint32_t& rootParameterIndex) {
	BaseLight::Draw(commandList, rootParameterIndex);
}

#ifdef _DEBUG
void SpotLight::Debug_Gui() {
	ImGui::ColorEdit4("color", &parameter_.color.x);
	ImGui::DragFloat3("position", &parameter_.position.x, 0.1f);
	ImGui::DragFloat("intensity", &parameter_.intensity, 0.1f, 0.0f, 1.0f);
	ImGui::DragFloat3("direction", &parameter_.direction.x, 0.1f, -1.0f, 1.0f);
	ImGui::DragFloat("distance", &parameter_.distance, 0.1f, 0.0f, 10.0f);
	ImGui::DragFloat("decay", &parameter_.decay, 0.1f, 0.0f, 1.0f);
	ImGui::SliderFloat("cosAngle", &cosDegree_, 0.0f, (std::numbers::pi_v<float>));
	ImGui::SliderFloat("falloffDegree ", &falloffDegree_, 0.0f, (std::numbers::pi_v<float>));


	parameter_.direction = Normalize(parameter_.direction);
	parameter_.cosAngle = std::cos(cosDegree_);
	parameter_.cosFalloffStart = std::cos(falloffDegree_);

	spotLightData_->color = parameter_.color;
	spotLightData_->position = parameter_.position;
	spotLightData_->intensity = parameter_.intensity;
	spotLightData_->direction = parameter_.direction;
	spotLightData_->distance = parameter_.distance;
	spotLightData_->decay = parameter_.decay;
	spotLightData_->cosAngle = parameter_.cosAngle;
	spotLightData_->cosFalloffStart = parameter_.cosFalloffStart;

	if (spotLightData_->cosFalloffStart <= spotLightData_->cosAngle) {
		spotLightData_->cosFalloffStart = spotLightData_->cosAngle + 0.01f;
	}

	if (ImGui::Button("Save")) {
		AdjustmentItem::Save("Light", parameter_.ToJson("spotLight"));
	}
	if (ImGui::Button("Apply")) {
		parameter_.FromJson(AdjustmentItem::GetData("Light", "spotLight"));
	}
}
#endif