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

ComPtr<IDxcBlob> DirectXCompiler::VsShaderCompile(const std::string& shader) {
	ComPtr<IDxcBlob> result{};
	result = CompilerShader(ConvertWString(shader), L"main", L"vs_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(result != nullptr);

	return result.Get();
}

ComPtr<IDxcBlob> DirectXCompiler::PsShaderCompile(const std::string& shader) {
	ComPtr<IDxcBlob> result{};
	result = CompilerShader(ConvertWString(shader), L"main", L"ps_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(result != nullptr);

	return result.Get();
}

ComPtr<IDxcBlob> DirectXCompiler::CsShaderCompile(const std::string& shader) {
	ComPtr<IDxcBlob> result{};
	result = CompilerShader(ConvertWString(shader), L"CSmain", L"cs_6_0", dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(result != nullptr);

	return result.Get();
}
