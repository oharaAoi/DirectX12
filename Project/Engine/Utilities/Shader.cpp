#include "Shader.h"

Shader::Shader() {
}

Shader::~Shader() {
}

void Shader::Init() {
	// VS/PS
	Load("Engine/HLSL/Object3d.VS.hlsl", "Engine/HLSL/Object3d.PS.hlsl", Shader::Normal);
	Load("Engine/HLSL/Object3d.VS.hlsl", "Engine/HLSL/Textureless.PS.hlsl", Shader::TextureLess);
	Load("Engine/HLSL/Primitive.VS.hlsl", "Engine/HLSL/Primitive.PS.hlsl", Shader::Primitive);
	Load("Engine/HLSL/Object3d.VS.hlsl", "Engine/HLSL/Phong.Lighting.hlsl", Shader::Phong);
	Load("Engine/HLSL/PBR.VS.hlsl", "Engine/HLSL/PBR.PS.hlsl", Shader::PBR);
	Load("Engine/HLSL/Sprite.VS.hlsl", "Engine/HLSL/Sprite.PS.hlsl", Shader::Sprite);
	Load("Engine/HLSL/Particle/Particle.VS.hlsl", "Engine/HLSL/Particle/Particle.PS.hlsl", Shader::Particle);

	// CS
	Load("Engine/HLSL/CS/GrayScale.CS.hlsl", CsShaderName::GrayScale);
	Load("Engine/HLSL/CS/HorizontalBlur.CS.hlsl", CsShaderName::HorizontalBlur);
	Load("Engine/HLSL/CS/VerticalBlur.CS.hlsl", CsShaderName::VerticalBlur);
	Load("Engine/HLSL/CS/DepthOfField.CS.hlsl", CsShaderName::DepthOfField);
	Load("Engine/HLSL/CS/Skinning.CS.hlsl", CsShaderName::SkinningCS);
	Load("Engine/HLSL/CS/Blend.CS.hlsl", CsShaderName::Blend);
	Load("Engine/HLSL/CS/ResultRender.CS.hlsl", CsShaderName::Result);

	Load("Engine/HLSL/Particle/GpuParticleInit.CS.hlsl", CsShaderName::GpuParticleInit);
	Load("Engine/HLSL/Particle/GpuParticleUpdate.CS.hlsl", CsShaderName::GpuParticleUpdate);
	Load("Engine/HLSL/Particle/GpuParticleEnd.CS.hlsl", CsShaderName::GpuParticleEnd);
	Load("Engine/HLSL/Particle/EmitParticle.CS.hlsl", CsShaderName::EmitGpuParticle);
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
