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
	Load("Engine/HLSL/Particle.VS.hlsl", "Engine/HLSL/Particle.PS.hlsl", Shader::Particle);
	Load("Engine/HLSL/Sprite.VS.hlsl", "Engine/HLSL/Sprite.PS.hlsl", Shader::Sprite);
	Load("Engine/HLSL/Object3dSimple.VS.hlsl", "Engine/HLSL/Object3d.PS.hlsl", Shader::LocalVertexOutput);

	// CS
	Load("Engine/HLSL/GrayScale.CS.hlsl", CsShaderName::GrayScale);
	Load("Engine/HLSL/HorizontalBlur.CS.hlsl", CsShaderName::HorizontalBlur);
	Load("Engine/HLSL/VerticalBlur.CS.hlsl", CsShaderName::VerticalBlur);
	Load("Engine/HLSL/DepthOfField.CS.hlsl", CsShaderName::DepthOfField);
	Load("Engine/HLSL/Skinning.CS.hlsl", CsShaderName::SkinningCS);
	Load("Engine/HLSL/Blend.CS.hlsl", CsShaderName::Blend);
	Load("Engine/HLSL/ResultRender.CS.hlsl", CsShaderName::Result);

	Load("Engine/HLSL/GpuParticleInit.CS.hlsl", CsShaderName::GpuParticleInit);
	Load("Engine/HLSL/GpuParticleUpdate.CS.hlsl", CsShaderName::GpuParticleUpdate);
	Load("Engine/HLSL/EmitParticle.CS.hlsl", CsShaderName::EmitGpuParticle);

	// GS
	Load("Engine/HLSL/TriangleSeparation.GS.hlsl", GeometoryShaderName::TriangleSeparation);
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

void Shader::Load(const std::string& gsPath, const GeometoryShaderName& type) {
	gsData_[type] = gsPath;
}
