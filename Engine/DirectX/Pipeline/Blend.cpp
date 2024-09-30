#include "Blend.h"

Blend::Blend() {
}

Blend::~Blend() {
}

void Blend::Init() {
}

D3D12_BLEND_DESC Blend::SetBlend(const BlendMode& mode) {
	D3D12_BLEND_DESC blendDesc{};
	// すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

	switch (mode) {
	case BlendMode::kBlendModeNone:

		break;
	case BlendMode::kBlendModeNormal:
		SetNormalBlend(blendDesc);
		break;
	case BlendMode::kBlendModeAdd:
		SetAddBlend(blendDesc);
		break;
	case BlendMode::kBlendModeSubtract:
		SetSubtractBlend(blendDesc);
		break;
	case BlendMode::kBlendModeMultily:
		SetMultiplyBlend(blendDesc);
		break;
	case BlendMode::kBlendModeScreen:
		SetScreenBlend(blendDesc);
		break;
	}

	return blendDesc;
}

void Blend::SetNormalBlend(D3D12_BLEND_DESC& blendDesc) {
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
}

void Blend::SetAddBlend(D3D12_BLEND_DESC& blendDesc) {
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
}

void Blend::SetSubtractBlend(D3D12_BLEND_DESC& blendDesc) {
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_SUBTRACT;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
}

void Blend::SetMultiplyBlend(D3D12_BLEND_DESC& blendDesc) {
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;
}

void Blend::SetScreenBlend(D3D12_BLEND_DESC& blendDesc) {
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
}
