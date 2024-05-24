#include "LightGroup.h"

LightGroup::LightGroup() {
}

LightGroup::~LightGroup() {
}

void LightGroup::Init(ID3D12Device* device) {
	light_ = std::make_unique<Light>();

	light_->Init(device);
}

void LightGroup::Finalize() {
	light_->Finalize();
}

void LightGroup::Update() {
}

void LightGroup::Draw(ID3D12GraphicsCommandList* commandList, const uint32_t& rootParameterIndex, const LightKind& kind) {
	switch (kind) {
	case LightKind::Directional:
		light_->Draw(commandList, rootParameterIndex);
		break;
	}
}
