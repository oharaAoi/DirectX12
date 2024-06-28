#include "LightGroup.h"

LightGroup::LightGroup() {
}

LightGroup::~LightGroup() {
}

void LightGroup::Init(ID3D12Device* device) {
	light_ = std::make_unique<Light>();
	phong_ = std::make_unique<Phong>();

	light_->Init(device);
	phong_->Init(device);
}

void LightGroup::Finalize() {
	light_->Finalize();
	phong_->Finalize();
}

void LightGroup::Update() {
	phong_->SetEyePos(eyePos_);
	light_->SetEyePos(eyePos_);

	light_->ImGuiDraw();
}

void LightGroup::Draw(ID3D12GraphicsCommandList* commandList, const uint32_t& rootParameterIndex, const LightKind& kind) {
	switch (kind) {
	case LightKind::Directional:
		light_->Draw(commandList, rootParameterIndex);
		break;
	case LightKind::PBR:
		light_->Draw(commandList, rootParameterIndex);
		break;
	}
}
