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
		Particle
	};

	// 構造体
	struct ShaderData {
		std::string vsShader;
		std::string psShader;
	};

public:

	Shader();
	~Shader();

	void Load(const std::string& vsPath, const std::string& psPath, const ShaderName& type);

	ShaderData GetShaderData(const ShaderName& name) { return shaderData_[name]; }

private:

	// 名前を保存
	std::map<ShaderName, ShaderData> shaderData_;

};

