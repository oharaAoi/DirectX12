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

	// 頂点のワールド座標
	elementDesc.SemanticName = "WORLDPOS";
	elementDesc.SemanticIndex = 0;
	elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	elementDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	elementDescs.push_back(elementDesc);

	// 法線の接線ベクトル
	elementDesc.SemanticName = "TANGENT";
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

std::vector<D3D12_INPUT_ELEMENT_DESC> InputLayout::CreateParticleInputLayout() {
	std::vector<D3D12_INPUT_ELEMENT_DESC> elementDescs = {};
	D3D12_INPUT_ELEMENT_DESC elementDesc{};
	elementDesc.SemanticName = "POSITION";
	elementDesc.SemanticIndex = 0;
	elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	elementDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	elementDescs.push_back(elementDesc);

	elementDesc.SemanticName = "TEXCOORD";
	elementDesc.SemanticIndex = 0;
	elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
	elementDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	elementDescs.push_back(elementDesc);

	elementDesc.SemanticName = "COLOR";
	elementDesc.SemanticIndex = 0;
	elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	elementDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	elementDescs.push_back(elementDesc);


	return elementDescs;
}

std::vector<D3D12_INPUT_ELEMENT_DESC> InputLayout::CreateSpriteInputLayout() {
	std::vector<D3D12_INPUT_ELEMENT_DESC> elementDescs = {};
	D3D12_INPUT_ELEMENT_DESC elementDesc{};
	elementDesc.SemanticName = "POSITION";
	elementDesc.SemanticIndex = 0;
	elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	elementDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	elementDescs.push_back(elementDesc);

	elementDesc.SemanticName = "TEXCOORD";
	elementDesc.SemanticIndex = 0;
	elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
	elementDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	elementDescs.push_back(elementDesc);

	return elementDescs;
}

std::vector<D3D12_INPUT_ELEMENT_DESC> InputLayout::CreateSkinningInputLayout() {
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

	// 頂点のワールド座標
	elementDesc.SemanticName = "WORLDPOS";
	elementDesc.SemanticIndex = 0;
	elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	elementDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	elementDescs.push_back(elementDesc);

	// 法線の接線ベクトル
	elementDesc.SemanticName = "TANGENT";
	elementDesc.SemanticIndex = 0;
	elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	elementDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	elementDescs.push_back(elementDesc);

	elementDesc.SemanticName = "WEIGHT";
	elementDesc.SemanticIndex = 0;
	elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	elementDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	elementDesc.InputSlot = 1;
	elementDescs.push_back(elementDesc);

	elementDesc.SemanticName = "INDEX";
	elementDesc.SemanticIndex = 0;
	elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
	elementDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	elementDesc.InputSlot = 1;
	elementDescs.push_back(elementDesc);

	return elementDescs;
}
