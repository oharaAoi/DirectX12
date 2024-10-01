#pragma once
// DirectX
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <memory>
#include <vector>

class InputLayout {
public:

	InputLayout();
	~InputLayout();

	void Initialize();

	void Finalize();

	std::vector<D3D12_INPUT_ELEMENT_DESC> CreateInputLayout();

	std::vector<D3D12_INPUT_ELEMENT_DESC> CreatePrimitiveInputLayout();

	std::vector<D3D12_INPUT_ELEMENT_DESC> CreateParticleInputLayout();

	std::vector<D3D12_INPUT_ELEMENT_DESC> CreateSpriteInputLayout();

	std::vector<D3D12_INPUT_ELEMENT_DESC> CreateSkinningInputLayout();
};