#pragma once
#include <d3d12.h>
#include <dxcapi.h>
#include <wrl.h>
#include <cassert>
#include <string>
#include "DirectXUtils.h"

template<typename T>
using Comptr = Microsoft::WRL::ComPtr <T>;

class DirectXCompiler {
public:

	DirectXCompiler();
	~DirectXCompiler();

	void Initialize();

	void Finalize();

	Comptr<IDxcBlob> VsShaderCompile(const std::string& shader);
	Comptr<IDxcBlob> PsShaderCompile(const std::string& shader);
	Comptr<IDxcBlob> CsShaderCompile(const std::string& shader);

private:

	Comptr<IDxcUtils> dxcUtils_ = nullptr;
	Comptr<IDxcCompiler3> dxcCompiler_ = nullptr;
	Comptr<IDxcIncludeHandler> includeHandler_ = nullptr;
	
};