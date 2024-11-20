#pragma once
#include <cassert>
#include <string>
#include <map>
#include "DirectXUtils.h"

class Shader {
public:

	// 
	enum ShaderName {
		Normal,
		TextureLess,
		Primitive,
		Phong,
		PBR,
		Particle,
		Sprite,

		LocalVertexOutput
	};

	enum CsShaderName {
		GrayScale,
		HorizontalBlur,
		VerticalBlur,
		DepthOfField,
		SkinningCS,
		Blend,
		Result,

		GpuParticleInit,
		GpuParticleUpdate,
		EmitGpuParticle,
	};

	enum GeometoryShaderName {
		TriangleSeparation,
	};

	// 構造体
	struct ShaderData {
		std::string vsShader;
		std::string psShader;
	};

public:

	Shader();
	~Shader();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	void Load(const std::string& vsPath, const std::string& psPath, const ShaderName& type);

	void Load(const std::string& csPath, const CsShaderName& type);

	void Load(const std::string& gsPath, const GeometoryShaderName& type);

	const ShaderData GetShaderData(const ShaderName& name){
		return shaderData_[name];
	}

	const std::string GetCsShaderData(const CsShaderName& name) {
		return csShaderData_[name];
	}

	const std::string GetGSData(const GeometoryShaderName& name) {
		return gsData_[name];
	}

private:

	// 名前を保存
	std::map<ShaderName, ShaderData> shaderData_;

	std::map<CsShaderName, std::string> csShaderData_;

	std::map<GeometoryShaderName, std::string> gsData_;

};

