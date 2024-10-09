#pragma once
#include <unordered_map>
#include "Engine/Utilities/Convert.h"
#include "Engine/Utilities/DirectXUtils.h"
#include "Engine/Utilities/Debug.h"
#include "Engine/DirectX/RootSignature/RootSignatureBuilder.h"

enum class RootSignatureType {
	Normal,
	TextureLess,
	Skinnig,
	Primitive,
	Particle,
	Sprite,
	PBR,
	ComputeShader,
	ComputeShaderBlend,
	CSReultRenderBlend,
};

class RootSignature {
public:

	RootSignature();
	~RootSignature();

	void Initialize(ID3D12Device* device, const RootSignatureType& type);

	void Finalize();

	ComPtr<ID3D12RootSignature> CreateRootSignature();
	ComPtr<ID3D12RootSignature> CreateTexturelessRootSignature();
	ComPtr<ID3D12RootSignature> CreateSkinnigRootSignature();
	ComPtr<ID3D12RootSignature> CreatePrimitiveRootSignature();
	ComPtr<ID3D12RootSignature> CreateParticleRootSignature();
	ComPtr<ID3D12RootSignature> CreateSpriteRootSignature();
	ComPtr<ID3D12RootSignature> CreatePBRRootSignature();
	ComPtr<ID3D12RootSignature> CreateComputeShaderRootSignature();
	ComPtr<ID3D12RootSignature> CreateBlendShaderRootSignature();
	ComPtr<ID3D12RootSignature> CreateResultRenderRootSignature();

public:

	using FunctionPointer = ComPtr<ID3D12RootSignature>(RootSignature::*)();
	std::unordered_map<RootSignatureType, FunctionPointer> functionMap_ = {
		// graphics
		{RootSignatureType::Normal, &RootSignature::CreateRootSignature},
		{RootSignatureType::TextureLess, &RootSignature::CreateTexturelessRootSignature},
		{RootSignatureType::Skinnig, &RootSignature::CreateSkinnigRootSignature},
		{RootSignatureType::Primitive, &RootSignature::CreatePrimitiveRootSignature},
		{RootSignatureType::Particle, &RootSignature::CreateParticleRootSignature},
		{RootSignatureType::Sprite, &RootSignature::CreateSpriteRootSignature},
		{RootSignatureType::PBR, &RootSignature::CreatePBRRootSignature},
		// CS
		{RootSignatureType::ComputeShader, &RootSignature::CreateComputeShaderRootSignature},
		{RootSignatureType::ComputeShaderBlend, &RootSignature::CreateBlendShaderRootSignature},
		{RootSignatureType::CSReultRenderBlend, &RootSignature::CreateResultRenderRootSignature},
	};

	ComPtr<ID3D12RootSignature> Create(const RootSignatureType& type);

	D3D12_STATIC_SAMPLER_DESC CreateSampler();

public:

	ID3D12RootSignature* GetRootSignature() { return rootSignature_.Get(); }

private:

	// rootSignature
	ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;

	RootSignatureBuilder builder_;

	ID3D12Device* device_ = nullptr;
};