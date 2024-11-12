#pragma once
#include <vector>
#include "Engine/Utilities/Convert.h"
#include "Engine/Utilities/DirectXUtils.h"
#include "Engine/Utilities/Debug.h"

class RootSignatureBuilder {
public:

	RootSignatureBuilder();
	~RootSignatureBuilder();

	RootSignatureBuilder& AddCBV(const UINT& shaderRegister, const D3D12_SHADER_VISIBILITY& visibility);

	RootSignatureBuilder& AddSRV(const UINT& shaderRegister, const D3D12_SHADER_VISIBILITY& visibility);

	RootSignatureBuilder& AddDescriptorTable(D3D12_DESCRIPTOR_RANGE* descriptorRange, UINT rangeCount, D3D12_SHADER_VISIBILITY visibility);

	RootSignatureBuilder& AddSampler(const D3D12_STATIC_SAMPLER_DESC& samplerDesc);

	ComPtr<ID3D12RootSignature> Build(ID3D12Device* device);

private:
	std::vector<D3D12_ROOT_PARAMETER> rootParameters;
	std::vector<D3D12_STATIC_SAMPLER_DESC> staticSamplers;
	D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_NONE;
};

