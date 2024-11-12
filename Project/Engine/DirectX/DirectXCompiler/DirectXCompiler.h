#pragma once
#include <d3d12.h>
#include <dxcapi.h>
#include <wrl.h>
#include <cassert>
#include <string>
#include "Engine/Utilities/DirectXUtils.h"

template<typename T>
using ComPtr = Microsoft::WRL::ComPtr <T>;

class DirectXCompiler {
public:

	DirectXCompiler();
	~DirectXCompiler();

	void Initialize();

	void Finalize();

	ComPtr<IDxcBlob> VsShaderCompile(const std::string& shader);
	ComPtr<IDxcBlob> PsShaderCompile(const std::string& shader);
	ComPtr<IDxcBlob> CsShaderCompile(const std::string& shader);

private:

	ComPtr<IDxcUtils> dxcUtils_ = nullptr;
	ComPtr<IDxcCompiler3> dxcCompiler_ = nullptr;
	ComPtr<IDxcIncludeHandler> includeHandler_ = nullptr;
	
};