#include "Shader.h"

Shader::Shader() {
}

Shader::~Shader() {
}

void Shader::Init() {
	// VS/PS
	Load("Engine/Resources/HLSL/Object3d.VS.hlsl",			"Engine/Resources/HLSL/Object3d.PS.hlsl", Shader::Normal);
	Load("Engine/Resources/HLSL/Object3d.VS.hlsl",			"Engine/Resources/HLSL/Textureless.PS.hlsl", Shader::TextureLess);
	Load("Engine/Resources/HLSL/Primitive.VS.hlsl",			"Engine/Resources/HLSL/Primitive.PS.hlsl", Shader::Primitive);
	Load("Engine/Resources/HLSL/Object3d.VS.hlsl",			"Engine/Resources/HLSL/Phong.Lighting.hlsl", Shader::Phong);
	Load("Engine/Resources/HLSL/PBR.VS.hlsl",				"Engine/Resources/HLSL/PBR.PS.hlsl", Shader::PBR);
	Load("Engine/Resources/HLSL/Sprite.VS.hlsl",			"Engine/Resources/HLSL/Sprite.PS.hlsl", Shader::Sprite);
	Load("Engine/Resources/HLSL/Particle/Particle.VS.hlsl", "Engine/Resources/HLSL/Particle/Particle.PS.hlsl", Shader::Particle);

	// CS
	Load("Engine/Resources/HLSL/CS/GrayScale.CS.hlsl", CsShaderName::GrayScale);
	Load("Engine/Resources/HLSL/CS/HorizontalBlur.CS.hlsl", CsShaderName::HorizontalBlur);
	Load("Engine/Resources/HLSL/CS/VerticalBlur.CS.hlsl", CsShaderName::VerticalBlur);
	Load("Engine/Resources/HLSL/CS/DepthOfField.CS.hlsl", CsShaderName::DepthOfField);
	Load("Engine/Resources/HLSL/CS/Skinning.CS.hlsl", CsShaderName::SkinningCS);
	Load("Engine/Resources/HLSL/CS/Blend.CS.hlsl", CsShaderName::Blend);
	Load("Engine/Resources/HLSL/CS/ResultRender.CS.hlsl", CsShaderName::Result);

	Load("Engine/Resources/HLSL/Particle/GpuParticleInit.CS.hlsl", CsShaderName::GpuParticleInit);
	Load("Engine/Resources/HLSL/Particle/GpuParticleUpdate.CS.hlsl", CsShaderName::GpuParticleUpdate);
	Load("Engine/Resources/HLSL/Particle/GpuParticleEnd.CS.hlsl", CsShaderName::GpuParticleEnd);
	Load("Engine/Resources/HLSL/Particle/EmitParticle.CS.hlsl", CsShaderName::EmitGpuParticle);
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
