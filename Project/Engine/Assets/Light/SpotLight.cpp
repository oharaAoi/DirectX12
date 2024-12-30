#include "SpotLight.h"

SpotLight::SpotLight() {
}

SpotLight::~SpotLight() {
}

void SpotLight::Init(ID3D12Device* device, const size_t& size) {
	BaseLight::Init(device, size);
	lightBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&spotLightData_));
	spotLightData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	spotLightData_->position = { 2.0f, 1.25f, 0.0f };
	spotLightData_->intensity = 0.0f;
	spotLightData_->direction = Normalize({ -1.0f, -1.0f, 0.0f });
	spotLightData_->distance = 7.0f;
	spotLightData_->decay = 2.0f;
	spotLightData_->cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);
	spotLightData_->cosFalloffStart = std::cos(std::numbers::pi_v<float> / 4.0f);

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

void SpotLight::ImGuiDraw() {
#ifdef _DEBUG
	ImGui::Begin("SpotLight");
	ImGui::DragFloat3("position", &spotLightData_->position.x, 0.1f, -10.0f, 10.0f);
	ImGui::DragFloat("intensity", &spotLightData_->intensity, 0.1f, 0.0f, 10.0f);
	ImGui::DragFloat3("direction", &spotLightData_->direction.x, 0.1f, -10.0f, 10.0f);
	ImGui::DragFloat("distance", &spotLightData_->distance, 0.1f, 0.0f, 10.0f);
	ImGui::DragFloat("decay", &spotLightData_->decay, 0.1f, 0.0f, 1.0f);
	ImGui::SliderFloat("cosAngle", &cosDegree_, 0.0f, (std::numbers::pi_v<float>));
	ImGui::SliderFloat("falloffDegree ", &falloffDegree_, 0.0f, (std::numbers::pi_v<float>));
	ImGui::End();
#endif

	spotLightData_->direction = Normalize(spotLightData_->direction);
	spotLightData_->cosAngle = std::cos(cosDegree_);
	spotLightData_->cosFalloffStart = std::cos(falloffDegree_);

	if (spotLightData_->cosFalloffStart <= spotLightData_->cosAngle) {
		spotLightData_->cosFalloffStart = spotLightData_->cosAngle + 0.01f;
	}
}
