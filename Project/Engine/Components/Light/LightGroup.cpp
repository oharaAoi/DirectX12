#include "LightGroup.h"
#include "Engine/System/Editer/Window/EditerWindows.h"

LightGroup::LightGroup() {}
LightGroup::~LightGroup() {}

void LightGroup::Init(ID3D12Device* device) {
	directionalLight_ = std::make_unique<DirectionalLight>();
	pointLight_ = std::make_unique<PointLight>();
	spotLight_ = std::make_unique<SpotLight>();

	directionalLight_->Init(device, sizeof(DirectionalLight::DirectionalLightData));
	pointLight_->Init(device, sizeof(PointLight::PointLightData));
	spotLight_->Init(device, sizeof(SpotLight::SpotLightData));

	this->AddChild(directionalLight_.get());
	this->AddChild(pointLight_.get());
	this->AddChild(spotLight_.get());
	
#ifdef _DEBUG
	EditerWindows::AddObjectWindow(this, "LightGroup");
#endif
}

void LightGroup::Finalize() {
	directionalLight_->Finalize();
	pointLight_->Finalize();
	spotLight_->Finalize();
}

void LightGroup::Update() {
	directionalLight_->SetEyePos(eyePos_);
	pointLight_->SetEyePos(eyePos_);
	spotLight_->SetEyePos(eyePos_);

	directionalLight_->Update();
	pointLight_->Update();
	spotLight_->Update();
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

#ifdef _DEBUG
void LightGroup::Debug_Gui() {

}
#endif // _DEBUG