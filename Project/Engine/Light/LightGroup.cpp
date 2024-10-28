#include "LightGroup.h"

LightGroup::LightGroup() {
}

LightGroup::~LightGroup() {
}

void LightGroup::Init(ID3D12Device* device) {
	directionalLight_ = std::make_unique<DirectionalLight>();
	pointLight_ = std::make_unique<PointLight>();
	spotLight_ = std::make_unique<SpotLight>();
	phong_ = std::make_unique<Phong>();

	directionalLight_->Init(device, sizeof(DirectionalLight::DirectionalLightData));
	pointLight_->Init(device, sizeof(PointLight::PointLightData));
	spotLight_->Init(device, sizeof(SpotLight::SpotLightData));
	phong_->Init(device);
}

void LightGroup::Finalize() {
	directionalLight_->Finalize();
	pointLight_->Finalize();
	spotLight_->Finalize();
	phong_->Finalize();
}

void LightGroup::Update() {
	phong_->SetEyePos(eyePos_);
	directionalLight_->SetEyePos(eyePos_);
	pointLight_->SetEyePos(eyePos_);
	spotLight_->SetEyePos(eyePos_);

	/*directionalLight_->ImGuiDraw();
	pointLight_->ImGuiDraw();
	spotLight_->ImGuiDraw();*/
}

void LightGroup::Draw(ID3D12GraphicsCommandList* commandList, const uint32_t& rootParameterIndex) {
	directionalLight_->Draw(commandList, rootParameterIndex);
	pointLight_->Draw(commandList, rootParameterIndex + 1);
	spotLight_->Draw(commandList, rootParameterIndex + 2);
}

void LightGroup::DrawLi(ID3D12GraphicsCommandList* commandList, const uint32_t& rootParameterIndex) {
	directionalLight_->Draw(commandList, rootParameterIndex);
}

void LightGroup::DrawPar(ID3D12GraphicsCommandList* commandList, const uint32_t& rootParameterIndex) {
	directionalLight_->Draw(commandList, rootParameterIndex);
}
