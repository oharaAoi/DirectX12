#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include "dxgidebug.h"
#include <cassert>
#include <wrl.h>
// utilities
#include "Engine/Utilities/Convert.h"
#include "Engine/Utilities/DirectXUtils.h"
#include "Engine/Utilities/Debug.h"

template<typename T>
using ComPtr = Microsoft::WRL::ComPtr <T>;

enum class RootSignatureType {
	Normal,
	TextureLess,
	Skinnig,
	Primitive,
	Particle,
	Sprite,
	PBR,
	ComputeShader,
	ComputeShaderBlend
};

class RootSignature {
public:

	RootSignature(ID3D12Device* device, const RootSignatureType& type);
	~RootSignature();

	void Initialize(ID3D12Device* device, const RootSignatureType& type);

	void Finalize();

	/// <summary>
	/// RootSignatureの作成
	/// </summary>
	/// <returns></returns>
	ComPtr<ID3D12RootSignature> CreateRootSignature();

	/// <summary>
	/// textureのないRootSignatureの作成
	/// </summary>
	/// <returns></returns>
	ComPtr<ID3D12RootSignature> CreateTexturelessRootSignature();

	/// <summary>
	/// Skinnngを行うモデルのRootSignatureの作成
	/// </summary>
	/// <returns></returns>
	ComPtr<ID3D12RootSignature> CreateSkinnigRootSignature();

	/// <summary>
	/// primitiveRootSignatureの作成
	/// </summary>
	/// <returns></returns>
	ComPtr<ID3D12RootSignature> CreatePrimitiveRootSignature();

	/// <summary>
	/// particle用のrootSignatureの作成
	/// </summary>
	/// <returns></returns>
	ComPtr<ID3D12RootSignature> CreateParticleRootSignature();

	/// <summary>
	/// sprite用のrootSignatureの作成
	/// </summary>
	/// <returns></returns>
	ComPtr<ID3D12RootSignature> CreateSpriteRootSignature();

	/// <summary>
	/// RootSignatureの作成
	/// </summary>
	/// <returns></returns>
	ComPtr<ID3D12RootSignature> CreatePBRRootSignature();

	/// <summary>
	/// コンピュートシェーダ用のRootSignatureの作成
	/// </summary>
	/// <returns></returns>
	ComPtr<ID3D12RootSignature> CreateComputeShaderRootSignature();

	/// <summary>
	/// コンピュートシェーダ用
	/// </summary>
	/// <returns></returns>
	ComPtr<ID3D12RootSignature> CreateBlendShaderRootSignature();

public:

	ID3D12RootSignature* GetRootSignature() { return rootSignature_.Get(); }

private:

	// rootSignature
	ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;

	ID3D12Device* device_ = nullptr;
};