#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include "dxgidebug.h"
#include <cassert>
#include <wrl.h>
// utilities
#include "Convert.h"
#include "DirectXUtils.h"
#include "Debug.h"

template<typename T>
using Comptr = Microsoft::WRL::ComPtr <T>;

enum class RootSignatureType {
	Normal,
	TextureLess,
	Primitive,
	Particle,
	Sprite
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
	Microsoft::WRL::ComPtr<ID3D12RootSignature> CreateRootSignature();

	/// <summary>
	/// textureのないRootSignatureの作成
	/// </summary>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12RootSignature> CreateTexturelessRootSignature();

	/// <summary>
	/// primitiveRootSignatureの作成
	/// </summary>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12RootSignature> CreatePrimitiveRootSignature();

	/// <summary>
	/// particle用のrootSignatureの作成
	/// </summary>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12RootSignature> CreateParticleRootSignature();

	/// <summary>
	/// sprite用のrootSignatureの作成
	/// </summary>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12RootSignature> CreateSpriteRootSignature();

public:

	ID3D12RootSignature* GetRootSignature() { return rootSignature_.Get(); }

private:

	// rootSignature
	Comptr<ID3D12RootSignature> rootSignature_ = nullptr;

	ID3D12Device* device_ = nullptr;
};