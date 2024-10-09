#include "RootSignatureBuilder.h"

RootSignatureBuilder::RootSignatureBuilder() {
}

RootSignatureBuilder::~RootSignatureBuilder() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　定数バッファを作成する
//////////////////////////////////////////////////////////////////////////////////////////////////

RootSignatureBuilder& RootSignatureBuilder::AddCBV(const UINT& shaderRegister, const D3D12_SHADER_VISIBILITY& visibility) {
	D3D12_ROOT_PARAMETER param = {};
	param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	param.ShaderVisibility = visibility;
	param.Descriptor.ShaderRegister = shaderRegister;
	rootParameters.push_back(param);
	return *this;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　DescriptorTableを作成する
//////////////////////////////////////////////////////////////////////////////////////////////////

RootSignatureBuilder& RootSignatureBuilder::AddDescriptorTable(D3D12_DESCRIPTOR_RANGE* descriptorRange, UINT rangeCount, D3D12_SHADER_VISIBILITY visibility) {
	D3D12_ROOT_PARAMETER param = {};
	param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	param.ShaderVisibility = visibility;
	param.DescriptorTable.NumDescriptorRanges = rangeCount;
	param.DescriptorTable.pDescriptorRanges = descriptorRange;
	rootParameters.push_back(param);
	return *this;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　サンプラーの作成
//////////////////////////////////////////////////////////////////////////////////////////////////

RootSignatureBuilder& RootSignatureBuilder::AddSampler(const D3D12_STATIC_SAMPLER_DESC& samplerDesc) {
	D3D12_STATIC_SAMPLER_DESC staticSampler = {};
	staticSampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	staticSampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	staticSampler.MaxLOD = D3D12_FLOAT32_MAX;
	staticSampler.ShaderRegister = 0;
	staticSampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	staticSamplers.push_back(samplerDesc);
	return *this;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　実際にビルドする
//////////////////////////////////////////////////////////////////////////////////////////////////

ComPtr<ID3D12RootSignature> RootSignatureBuilder::Build(ID3D12Device* device) {
	D3D12_ROOT_SIGNATURE_DESC desc = {};
	desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	desc.NumParameters = static_cast<UINT>(rootParameters.size());
	desc.pParameters = rootParameters.data();
	desc.NumStaticSamplers = static_cast<UINT>(staticSamplers.size());
	desc.pStaticSamplers = staticSamplers.data();
	
	ComPtr<ID3DBlob> signatureBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;

	HRESULT hr = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		if (errorBlob) {
			Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		}
		assert(false);
	}

	ComPtr<ID3D12RootSignature> rootSignature;
	hr = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(hr));

	return rootSignature;
}