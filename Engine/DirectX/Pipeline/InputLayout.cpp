#include "InputLayout.h"

InputLayout::InputLayout() {}

InputLayout::~InputLayout() {}

void InputLayout::Initialize() {}

void InputLayout::Finalize() {}

std::vector<D3D12_INPUT_ELEMENT_DESC> InputLayout::CreateInputLayout() {
	// InputLayoutの生成
	std::vector<D3D12_INPUT_ELEMENT_DESC> elementDescs = {};

	D3D12_INPUT_ELEMENT_DESC elementDesc{};
	elementDesc.SemanticName = "POSITION";
	elementDesc.SemanticIndex = 0;
	elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	elementDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	elementDescs.push_back(elementDesc);

	// texture
	elementDesc.SemanticName = "TEXCOORD";
	elementDesc.SemanticIndex = 0;
	elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
	elementDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	elementDescs.push_back(elementDesc);

	// 法線
	elementDesc.SemanticName = "NORMAL";
	elementDesc.SemanticIndex = 0;
	elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	elementDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	elementDescs.push_back(elementDesc);

	return elementDescs;
}

std::vector<D3D12_INPUT_ELEMENT_DESC> InputLayout::CreatePrimitiveInputLayout() {
	std::vector<D3D12_INPUT_ELEMENT_DESC> elementDescs = {};
	D3D12_INPUT_ELEMENT_DESC elementDesc{};
	elementDesc.SemanticName = "POSITION";
	elementDesc.SemanticIndex = 0;
	elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	elementDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	elementDescs.push_back(elementDesc);

	elementDesc.SemanticName = "COLOR";
	elementDesc.SemanticIndex = 0;
	elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	elementDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	elementDescs.push_back(elementDesc);

	return elementDescs;
}
