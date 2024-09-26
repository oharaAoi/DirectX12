#include "EmissionEffect.h"

EmissionEffect::EmissionEffect() {
}

EmissionEffect::~EmissionEffect() {
}

void EmissionEffect::Finalize() {
	BaseEffect::Finalize();
}

void EmissionEffect::Init(const std::string& directoryPath, const std::string& fileName, const uint32_t& particleNum) {
	BaseEffect::Init(directoryPath, fileName, particleNum);
}

void EmissionEffect::Update(const Matrix4x4& viewMat, const Matrix4x4& projection) {
	BaseEffect::Update(viewMat, projection);

	BaseEffect::ImGuiDraw();
}

void EmissionEffect::Draw() {
	BaseEffect::Draw();
}
