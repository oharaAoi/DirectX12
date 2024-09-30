#include "Shader.h"

Shader::Shader() {
}

Shader::~Shader() {
}

void Shader::Init() {
	// VS/PS
	Load("Engine/HLSL/Object3d.VS.hlsl", "Engine/HLSL/Object3d.PS.hlsl", Shader::Normal);
	Load("Engine/HLSL/Object3d.VS.hlsl", "Engine/HLSL/Textureless.PS.hlsl", Shader::TextureLess);
	Load("Engine/HLSL/SkinningObject3d.VS.hlsl", "Engine/HLSL/Object3d.PS.hlsl", Shader::Skinning);
	Load("Engine/HLSL/Primitive.VS.hlsl", "Engine/HLSL/Primitive.PS.hlsl", Shader::Primitive);
	Load("Engine/HLSL/Object3d.VS.hlsl", "Engine/HLSL/Phong.Lighting.hlsl", Shader::Phong);
	Load("Engine/HLSL/PBR.VS.hlsl", "Engine/HLSL/PBR.PS.hlsl", Shader::PBR);
	Load("Engine/HLSL/Particle.VS.hlsl", "Engine/HLSL/Particle.PS.hlsl", Shader::Particle);
	Load("Engine/HLSL/Sprite.VS.hlsl", "Engine/HLSL/Sprite.PS.hlsl", Shader::Sprite);

	// CS
	Load("Engine/HLSL/GrayScale.CS.hlsl", CsShaderName::GrayScale);
	Load("Engine/HLSL/HorizontalBlur.CS.hlsl", CsShaderName::HorizontalBlur);
	Load("Engine/HLSL/VerticalBlur.CS.hlsl", CsShaderName::VerticalBlur);
	Load("Engine/HLSL/DepthOfField.CS.hlsl", CsShaderName::DepthOfField);
	Load("Engine/HLSL/Blend.CS.hlsl", CsShaderName::Blend);
}

void Shader::Load(const std::string& vsPath, const std::string& psPath, const ShaderName& type) {
	ShaderData data{};
	data.vsShader = vsPath;
	data.psShader = psPath;

	shaderData_[type] = data;
}

void Shader::Load(const std::string& csPath, const CsShaderName& type) {
	csShaderData_[type] = csPath;
}
