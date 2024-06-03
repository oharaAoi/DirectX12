#include "DirectXCompiler.h"
#pragma comment(lib, "dxcompiler.lib")

DirectXCompiler::DirectXCompiler() {
	Initialize();
}

DirectXCompiler::~DirectXCompiler() {
}

void DirectXCompiler::Initialize() {
	HRESULT hr = S_FALSE;
	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));

	// includeに対応するための設定
	hr = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
	assert(SUCCEEDED(hr));
}

void DirectXCompiler::Finalize() {
	dxcUtils_.Reset();
	dxcCompiler_.Reset();
	includeHandler_.Reset();
}

Comptr<IDxcBlob> DirectXCompiler::VsShaderCompile(const std::string& shader) {
	Comptr<IDxcBlob> result{};
	result = CompilerShader(ConvertWString(shader), L"vs_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(result != nullptr);

	return result.Get();
}

Comptr<IDxcBlob> DirectXCompiler::PsShaderCompile(const std::string& shader) {
	Comptr<IDxcBlob> result{};
	result = CompilerShader(ConvertWString(shader), L"ps_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(result != nullptr);

	return result.Get();
}
