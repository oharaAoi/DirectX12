#include "SpotLight.h"
#include "Engine/Utilities/AdjustmentItem.h"

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
	spotLightData_->direction = Normalize({ 0.0f, -0.9f, 0.3f });
	spotLightData_->distance = 7.0f;
	spotLightData_->decay = 2.0f;
	spotLightData_->cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);
	spotLightData_->cosFalloffStart = std::cos(std::numbers::pi_v<float> / 4.0f);

	cosDegree_ = 0.0f;
	falloffDegree_ = 0.0f;

	cosDegree_ = std::cos(std::numbers::pi_v<float> / 3.0f);
	falloffDegree_ = std::cos(std::numbers::pi_v<float> / 4.0f);

}

void SpotLight::Finalize() {
	BaseLight::Finalize();
}

void SpotLight::Update() {
	spotLightData_->direction = spotLightData_->direction.Normalize();
	spotLightData_->cosAngle = std::cos(cosDegree_);
	spotLightData_->cosFalloffStart = std::cos(falloffDegree_);

	if (spotLightData_->cosFalloffStart <= spotLightData_->cosAngle) {
		spotLightData_->cosFalloffStart = spotLightData_->cosAngle + 0.01f;
	}
}

void SpotLight::Draw(ID3D12GraphicsCommandList* commandList, const uint32_t& rootParameterIndex) {
	BaseLight::Draw(commandList, rootParameterIndex);
}

void SpotLight::AddAdjustment() {
	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	adjust->AddItem(groupName_, "pos", spotLightData_->position);					// 座標
	adjust->AddItem(groupName_, "direction", spotLightData_->direction);			// 方向
	adjust->AddItem(groupName_, "color", spotLightData_->color);					// ライトの色
	adjust->AddItem(groupName_, "distance", spotLightData_->distance);				// 光の当たる距離
	adjust->AddItem(groupName_, "intensity", spotLightData_->intensity);			// 輝度
	adjust->AddItem(groupName_, "decay", spotLightData_->decay);					// 光の減衰
	adjust->AddItem(groupName_, "cosAngle", cosDegree_);				// 光の余弦
	adjust->AddItem(groupName_, "cosFalloffStart", falloffDegree_);// 光の角度での減衰
}


void SpotLight::AdaptAdjustment() {
	AdjustmentItem* adjust = AdjustmentItem::GetInstance();
	spotLightData_->position = adjust->GetValue<Vector3>(groupName_, "pos");
	spotLightData_->direction = adjust->GetValue<Vector3>(groupName_, "direction");
	spotLightData_->color = adjust->GetValue<Vector4>(groupName_, "color");
	spotLightData_->distance = adjust->GetValue<float>(groupName_, "distance");
	spotLightData_->intensity = adjust->GetValue<float>(groupName_, "intensity");
	spotLightData_->decay = adjust->GetValue<float>(groupName_, "decay");
	cosDegree_ = adjust->GetValue<float>(groupName_, "cosAngle");
	falloffDegree_ = adjust->GetValue<float>(groupName_, "cosFalloffStart");
}

void SpotLight::ImGuiDraw() {
#ifdef _DEBUG
	ImGui::Begin("SpotLight");
	ImGui::DragFloat3("position", &spotLightData_->position.x, 0.1f, -50.0f, 50.0f);
	ImGui::DragFloat("intensity", &spotLightData_->intensity, 0.1f, 0.0f, 40.0f);
	ImGui::DragFloat3("direction", &spotLightData_->direction.x, 0.1f, -10.0f, 10.0f);
	ImGui::DragFloat("distance", &spotLightData_->distance, 0.1f, 0.0f, 100.0f);
	ImGui::DragFloat("decay", &spotLightData_->decay, 0.1f, 0.0f, 1.0f);
	ImGui::SliderFloat("cosAngle", &cosDegree_, 0.0f, (std::numbers::pi_v<float>));
	ImGui::SliderFloat("falloffDegree ", &falloffDegree_, 0.0f, (std::numbers::pi_v<float>));

	if (ImGui::Button("Save")) {
		AdjustmentItem* adjust = AdjustmentItem::GetInstance();
		adjust->SetValue(groupName_, "pos", spotLightData_->position);					// 座標
		adjust->SetValue(groupName_, "direction", spotLightData_->direction);			// 方向
		adjust->SetValue(groupName_, "color", spotLightData_->color);					// ライトの色
		adjust->SetValue(groupName_, "distance", spotLightData_->distance);				// 光の当たる距離
		adjust->SetValue(groupName_, "intensity", spotLightData_->intensity);			// 輝度
		adjust->SetValue(groupName_, "decay", spotLightData_->decay);					// 光の減衰
		adjust->SetValue(groupName_, "cosAngle", cosDegree_);				// 光の余弦
		adjust->SetValue(groupName_, "cosFalloffStart", falloffDegree_);// 光の角度での減衰
	}
	ImGui::SameLine();
	if (ImGui::Button("Adapt")) {
		AdaptAdjustment();
	}

	ImGui::End();

	spotLightData_->direction = Normalize(spotLightData_->direction);
	spotLightData_->cosAngle = std::cos(cosDegree_);
	spotLightData_->cosFalloffStart = std::cos(falloffDegree_);

	if (spotLightData_->cosFalloffStart <= spotLightData_->cosAngle) {
		spotLightData_->cosFalloffStart = spotLightData_->cosAngle + 0.01f;
	}
#endif
}
