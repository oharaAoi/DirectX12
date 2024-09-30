#pragma once
// DirectX
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>

class Blend {
public:

	enum BlendMode {
		// ブレンドなし
		kBlendModeNone,
		// 通常αブレンド
		kBlendModeNormal,
		// 加算
		kBlendModeAdd,
		// 減算
		kBlendModeSubtract,
		// 乗算
		kBlendModeMultily,
		//
		kBlendModeScreen
	};

public:

	Blend();
	~Blend();

	void Init();

	D3D12_BLEND_DESC SetBlend(const BlendMode& mode);

	void SetNormalBlend(D3D12_BLEND_DESC& blendDesc);

	void SetAddBlend(D3D12_BLEND_DESC& blendDesc);

	void SetSubtractBlend(D3D12_BLEND_DESC& blendDesc);

	void SetMultiplyBlend(D3D12_BLEND_DESC& blendDesc);

	void SetScreenBlend(D3D12_BLEND_DESC& blendDesc);

private:



};

