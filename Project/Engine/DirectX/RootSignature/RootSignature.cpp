#include "RootSignature.h"

RootSignature::RootSignature() {}
RootSignature::~RootSignature() {}

void RootSignature::Finalize() {
	rootSignature_.Reset();
}

void RootSignature::Initialize(ID3D12Device* device, const RootSignatureType& type) {
	device_ = device;
	rootSignature_ = Create(type);
}

//////////////////////////////////////////////////////////////////////////////////////
// 引数のrootsignatureを作成する
//////////////////////////////////////////////////////////////////////////////////////

ComPtr<ID3D12RootSignature> RootSignature::Create(const RootSignatureType& type) {
	const auto& it = functionMap_.find(type);
	if (it == functionMap_.end()) {
		assert("not find RootSignature");
	}

	return (this->*(it->second))(); // メンバー関数ポインタを使って関数を呼び出す
}

//////////////////////////////////////////////////////////////////////////////////////
// samplerの作成
//////////////////////////////////////////////////////////////////////////////////////

D3D12_STATIC_SAMPLER_DESC RootSignature::CreateSampler(D3D12_TEXTURE_ADDRESS_MODE mode) {
	D3D12_STATIC_SAMPLER_DESC spriteStaticSampler = {};
	spriteStaticSampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	spriteStaticSampler.AddressU = mode;
	spriteStaticSampler.AddressV = mode;
	spriteStaticSampler.AddressW = mode;
	spriteStaticSampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	spriteStaticSampler.MaxLOD = D3D12_FLOAT32_MAX;
	spriteStaticSampler.ShaderRegister = 0;
	spriteStaticSampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	
	
	return spriteStaticSampler;
}

//////////////////////////////////////////////////////////////////////////////////////
// 基本のRootSignature
//////////////////////////////////////////////////////////////////////////////////////

ComPtr<ID3D12RootSignature> RootSignature::CreateRootSignature() {
	D3D12_DESCRIPTOR_RANGE spriteDescriptorRange[1] = {};
	spriteDescriptorRange[0].BaseShaderRegister = 0;
	spriteDescriptorRange[0].NumDescriptors = 1;
	spriteDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	spriteDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	return builder_
		.AddCBV(0, D3D12_SHADER_VISIBILITY_PIXEL)  // Material用
		.AddCBV(0, D3D12_SHADER_VISIBILITY_VERTEX) // Transform用
		.AddCBV(1, D3D12_SHADER_VISIBILITY_VERTEX) // viewProjection用
		.AddDescriptorTable(spriteDescriptorRange, 1, D3D12_SHADER_VISIBILITY_PIXEL) // Texture用
		.AddCBV(1, D3D12_SHADER_VISIBILITY_PIXEL)  // directionalLight用
		.AddCBV(2, D3D12_SHADER_VISIBILITY_PIXEL)  // pointLight用
		.AddCBV(3, D3D12_SHADER_VISIBILITY_PIXEL)  // spotLight用
		.AddSampler(CreateSampler(D3D12_TEXTURE_ADDRESS_MODE_WRAP))
		.Build(device_);
}

//////////////////////////////////////////////////////////////////////////////////////
// textureがないRootSignature
//////////////////////////////////////////////////////////////////////////////////////

ComPtr<ID3D12RootSignature> RootSignature::CreateTexturelessRootSignature() {
	D3D12_DESCRIPTOR_RANGE spriteDescriptorRange[1] = {};
	spriteDescriptorRange[0].BaseShaderRegister = 0;
	spriteDescriptorRange[0].NumDescriptors = 1;
	spriteDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	spriteDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	return builder_
		.AddCBV(0, D3D12_SHADER_VISIBILITY_PIXEL)  // Material用
		.AddCBV(0, D3D12_SHADER_VISIBILITY_VERTEX) // Transform用
		.AddCBV(1, D3D12_SHADER_VISIBILITY_VERTEX) // viewProjection用
		.AddCBV(1, D3D12_SHADER_VISIBILITY_PIXEL)  // directionalLight用
		.AddCBV(2, D3D12_SHADER_VISIBILITY_PIXEL)  // pointLight用
		.AddCBV(3, D3D12_SHADER_VISIBILITY_PIXEL)  // spotLight用
		.AddSampler(CreateSampler(D3D12_TEXTURE_ADDRESS_MODE_WRAP))
		.Build(device_);
}

//////////////////////////////////////////////////////////////////////////////////////
// skinning
//////////////////////////////////////////////////////////////////////////////////////

ComPtr<ID3D12RootSignature> RootSignature::CreateSkinnigRootSignature() {
	D3D12_DESCRIPTOR_RANGE spriteDescriptorRange[1] = {};
	spriteDescriptorRange[0].BaseShaderRegister = 0;
	spriteDescriptorRange[0].NumDescriptors = 1;
	spriteDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	spriteDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE skinningDescriptorRange[1] = {};
	skinningDescriptorRange[0].BaseShaderRegister = 0;
	skinningDescriptorRange[0].NumDescriptors = 1;
	skinningDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	skinningDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	return builder_
		.AddCBV(0, D3D12_SHADER_VISIBILITY_PIXEL)  // Material用
		.AddCBV(0, D3D12_SHADER_VISIBILITY_VERTEX) // Transform用
		.AddCBV(1, D3D12_SHADER_VISIBILITY_VERTEX) // viewProjection用
		.AddDescriptorTable(spriteDescriptorRange, 1, D3D12_SHADER_VISIBILITY_PIXEL) // Texture用
		.AddDescriptorTable(skinningDescriptorRange, 1, D3D12_SHADER_VISIBILITY_VERTEX) // skinning用
		.AddCBV(1, D3D12_SHADER_VISIBILITY_PIXEL)  // directionalLight用
		.AddCBV(2, D3D12_SHADER_VISIBILITY_PIXEL)  // pointLight用
		.AddCBV(3, D3D12_SHADER_VISIBILITY_PIXEL)  // spotLight用
		.AddSampler(CreateSampler(D3D12_TEXTURE_ADDRESS_MODE_WRAP))
		.Build(device_);
}

//////////////////////////////////////////////////////////////////////////////////////
// primitiveRootSignatureの作成
//////////////////////////////////////////////////////////////////////////////////////
ComPtr<ID3D12RootSignature> RootSignature::CreatePrimitiveRootSignature() {
	return builder_
		.AddCBV(0, D3D12_SHADER_VISIBILITY_PIXEL)  // Material用
		.AddCBV(0, D3D12_SHADER_VISIBILITY_VERTEX) // Transform用
		.Build(device_);
}

//////////////////////////////////////////////////////////////////////////////////////
// particle
//////////////////////////////////////////////////////////////////////////////////////

ComPtr<ID3D12RootSignature> RootSignature::CreateParticleRootSignature() {
	D3D12_DESCRIPTOR_RANGE spriteDescriptorRange[1] = {};
	spriteDescriptorRange[0].BaseShaderRegister = 0;
	spriteDescriptorRange[0].NumDescriptors = 1;
	spriteDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	spriteDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing[1] = {};
	descriptorRangeForInstancing[0].BaseShaderRegister = 0;
	descriptorRangeForInstancing[0].NumDescriptors = 1;
	descriptorRangeForInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeForInstancing[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	return builder_
		.AddCBV(0, D3D12_SHADER_VISIBILITY_PIXEL)  // Material用
		.AddDescriptorTable(descriptorRangeForInstancing, 1, D3D12_SHADER_VISIBILITY_VERTEX) // Instancing描画用
		.AddDescriptorTable(spriteDescriptorRange, 1, D3D12_SHADER_VISIBILITY_PIXEL) // texture用
		.AddCBV(1, D3D12_SHADER_VISIBILITY_PIXEL)  // Light用
		.AddSampler(CreateSampler(D3D12_TEXTURE_ADDRESS_MODE_WRAP))
		.Build(device_);
}

//////////////////////////////////////////////////////////////////////////////////////
// sprite
//////////////////////////////////////////////////////////////////////////////////////

ComPtr<ID3D12RootSignature> RootSignature::CreateSpriteRootSignature() {
	D3D12_DESCRIPTOR_RANGE spriteDescriptorRange[1] = {};
	spriteDescriptorRange[0].BaseShaderRegister = 0;
	spriteDescriptorRange[0].NumDescriptors = 1;
	spriteDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	spriteDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	return builder_
		.AddCBV(0, D3D12_SHADER_VISIBILITY_PIXEL)  // Material用
		.AddCBV(0, D3D12_SHADER_VISIBILITY_VERTEX) // Transform用
		.AddDescriptorTable(spriteDescriptorRange, 1, D3D12_SHADER_VISIBILITY_PIXEL) // Texture用
		.AddSampler(CreateSampler(D3D12_TEXTURE_ADDRESS_MODE_BORDER))
		.Build(device_);
}

//////////////////////////////////////////////////////////////////////////////////////
// PBR
//////////////////////////////////////////////////////////////////////////////////////

ComPtr<ID3D12RootSignature> RootSignature::CreatePBRRootSignature() {
	D3D12_DESCRIPTOR_RANGE spriteDescriptorRange[1] = {};
	spriteDescriptorRange[0].BaseShaderRegister = 0;
	spriteDescriptorRange[0].NumDescriptors = 2;
	spriteDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	spriteDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	return builder_
		.AddCBV(0, D3D12_SHADER_VISIBILITY_PIXEL)  // Material用
		.AddCBV(0, D3D12_SHADER_VISIBILITY_VERTEX) // Transform用
		.AddCBV(1, D3D12_SHADER_VISIBILITY_VERTEX) // viewProjection用
		.AddDescriptorTable(spriteDescriptorRange, _countof(spriteDescriptorRange), D3D12_SHADER_VISIBILITY_PIXEL) // Texture用
		.AddCBV(1, D3D12_SHADER_VISIBILITY_PIXEL)  // directionalLight用
		.AddCBV(2, D3D12_SHADER_VISIBILITY_PIXEL)  // pointLight用
		.AddCBV(3, D3D12_SHADER_VISIBILITY_PIXEL)  // spotLight用
		.AddSampler(CreateSampler(D3D12_TEXTURE_ADDRESS_MODE_WRAP))
		.Build(device_);
}

//////////////////////////////////////////////////////////////////////////////////////
// CS
//////////////////////////////////////////////////////////////////////////////////////

ComPtr<ID3D12RootSignature> RootSignature::CreateComputeShaderRootSignature() {
	D3D12_DESCRIPTOR_RANGE spriteDescriptorRange[1] = {};
	spriteDescriptorRange[0].BaseShaderRegister = 0;
	spriteDescriptorRange[0].NumDescriptors = 1;
	spriteDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	spriteDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// 出力のストラクチャードバッファ
	D3D12_DESCRIPTOR_RANGE descriptorRangeUAV[1] = {};
	descriptorRangeUAV[0].BaseShaderRegister = 0;
	descriptorRangeUAV[0].NumDescriptors = 1;
	descriptorRangeUAV[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	descriptorRangeUAV[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	return builder_
		.AddDescriptorTable(spriteDescriptorRange, 1, D3D12_SHADER_VISIBILITY_ALL) // 参照用
		.AddDescriptorTable(descriptorRangeUAV, 1, D3D12_SHADER_VISIBILITY_ALL) // 出力用
		.AddCBV(0, D3D12_SHADER_VISIBILITY_ALL)  // 参照用
		.AddSampler(CreateSampler(D3D12_TEXTURE_ADDRESS_MODE_WRAP))
		.Build(device_);
}

ComPtr<ID3D12RootSignature> RootSignature::CreateCsSkinnigRootSignature() {
	/*D3D12_DESCRIPTOR_RANGE matrixPalette[1] = {};
	matrixPalette[0].BaseShaderRegister = 0;
	matrixPalette[0].NumDescriptors = 1;
	matrixPalette[0].RangeType = D3D12_SHADER_VISIBILITY_ALL;
	matrixPalette[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;*/

	D3D12_DESCRIPTOR_RANGE MatrixPalette[1] = {};
	MatrixPalette[0].BaseShaderRegister = 0;
	MatrixPalette[0].NumDescriptors = 1;
	MatrixPalette[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	MatrixPalette[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE  Vertex[1] = {};
	 Vertex[0].BaseShaderRegister = 1;
	 Vertex[0].NumDescriptors = 1;
	 Vertex[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	 Vertex[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	 D3D12_DESCRIPTOR_RANGE  VertexInfluence[1] = {};
	 VertexInfluence[0].BaseShaderRegister = 2;
	 VertexInfluence[0].NumDescriptors = 1;
	 VertexInfluence[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	 VertexInfluence[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// 出力のストラクチャードバッファ
	D3D12_DESCRIPTOR_RANGE descriptorRangeUAV[1] = {};
	descriptorRangeUAV[0].BaseShaderRegister = 0;
	descriptorRangeUAV[0].NumDescriptors = 1;
	descriptorRangeUAV[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	descriptorRangeUAV[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	
	return builder_
		.AddDescriptorTable(MatrixPalette, 1, D3D12_SHADER_VISIBILITY_ALL)  // MatrixPalette
		.AddDescriptorTable(Vertex, 1, D3D12_SHADER_VISIBILITY_ALL)  // Vertex
		.AddDescriptorTable(VertexInfluence, 1, D3D12_SHADER_VISIBILITY_ALL)  // VertexInfluence
		.AddDescriptorTable(descriptorRangeUAV, 1, D3D12_SHADER_VISIBILITY_ALL) // outpur
		.AddCBV(0, D3D12_SHADER_VISIBILITY_ALL)  // SkinningInformation 
		.Build(device_);
}

//////////////////////////////////////////////////////////////////////////////////////
// CSを合成する
//////////////////////////////////////////////////////////////////////////////////////

ComPtr<ID3D12RootSignature> RootSignature::CreateBlendShaderRootSignature() {
	D3D12_DESCRIPTOR_RANGE spriteDescriptorRange[1] = {};
	spriteDescriptorRange[0].BaseShaderRegister = 0;
	spriteDescriptorRange[0].NumDescriptors = 1;
	spriteDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	spriteDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// 出力のストラクチャードバッファ
	D3D12_DESCRIPTOR_RANGE descriptorRangeUAV[1] = {};
	descriptorRangeUAV[0].BaseShaderRegister = 0;
	descriptorRangeUAV[0].NumDescriptors = 1;
	descriptorRangeUAV[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	descriptorRangeUAV[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	return builder_
		.AddDescriptorTable(spriteDescriptorRange, 1, D3D12_SHADER_VISIBILITY_ALL) // 参照用
		.AddDescriptorTable(descriptorRangeUAV, 1, D3D12_SHADER_VISIBILITY_ALL) // 出力用
		.AddSampler(CreateSampler(D3D12_TEXTURE_ADDRESS_MODE_WRAP))
		.Build(device_);
}

//////////////////////////////////////////////////////////////////////////////////////
// 3Dと2Dの合成
//////////////////////////////////////////////////////////////////////////////////////

ComPtr<ID3D12RootSignature> RootSignature::CreateResultRenderRootSignature() {
	D3D12_DESCRIPTOR_RANGE object3DDescriptorRange[1] = {};
	object3DDescriptorRange[0].BaseShaderRegister = 0;
	object3DDescriptorRange[0].NumDescriptors = 1;
	object3DDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	object3DDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE spriteDescriptorRange[1] = {};
	spriteDescriptorRange[0].BaseShaderRegister = 1;
	spriteDescriptorRange[0].NumDescriptors = 1;
	spriteDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	spriteDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// 出力のストラクチャードバッファ
	D3D12_DESCRIPTOR_RANGE descriptorRangeUAV[1] = {};
	descriptorRangeUAV[0].BaseShaderRegister = 0;
	descriptorRangeUAV[0].NumDescriptors = 1;
	descriptorRangeUAV[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	descriptorRangeUAV[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	return builder_
		.AddDescriptorTable(object3DDescriptorRange, 1, D3D12_SHADER_VISIBILITY_ALL) // object3d用
		.AddDescriptorTable(spriteDescriptorRange, 1, D3D12_SHADER_VISIBILITY_ALL) // sprite用
		.AddDescriptorTable(descriptorRangeUAV, 1, D3D12_SHADER_VISIBILITY_ALL) // 出力用
		.AddSampler(CreateSampler(D3D12_TEXTURE_ADDRESS_MODE_WRAP))
		.Build(device_);
}

//////////////////////////////////////////////////////////////////////////////////////
// 波の動き
//////////////////////////////////////////////////////////////////////////////////////

ComPtr<ID3D12RootSignature> RootSignature::CreateWaterSpaceRootSignature() {
	D3D12_DESCRIPTOR_RANGE spriteDescriptorRange[1] = {};
	spriteDescriptorRange[0].BaseShaderRegister = 0;
	spriteDescriptorRange[0].NumDescriptors = 1;
	spriteDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	spriteDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	return builder_
		.AddCBV(0, D3D12_SHADER_VISIBILITY_PIXEL)  // Material用
		.AddCBV(0, D3D12_SHADER_VISIBILITY_VERTEX) // Transform用
		.AddCBV(1, D3D12_SHADER_VISIBILITY_VERTEX) // viewProjection用
		.AddDescriptorTable(spriteDescriptorRange, 1, D3D12_SHADER_VISIBILITY_PIXEL) // Texture用
		.AddCBV(2, D3D12_SHADER_VISIBILITY_VERTEX) // waveParameter用
		.AddCBV(1, D3D12_SHADER_VISIBILITY_PIXEL)  // directionalLight用
		.AddCBV(2, D3D12_SHADER_VISIBILITY_PIXEL)  // pointLight用
		.AddCBV(3, D3D12_SHADER_VISIBILITY_PIXEL)  // spotLight用
		.AddSampler(CreateSampler(D3D12_TEXTURE_ADDRESS_MODE_WRAP))
		.Build(device_);
}

ComPtr<ID3D12RootSignature> RootSignature::CreateWaterLightingRootSignature() {
	D3D12_DESCRIPTOR_RANGE spriteDescriptorRange[1] = {};
	spriteDescriptorRange[0].BaseShaderRegister = 0;
	spriteDescriptorRange[0].NumDescriptors = 2;
	spriteDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	spriteDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	return builder_
		.AddCBV(0, D3D12_SHADER_VISIBILITY_PIXEL)  // Material用
		.AddCBV(0, D3D12_SHADER_VISIBILITY_VERTEX) // Transform用
		.AddCBV(1, D3D12_SHADER_VISIBILITY_VERTEX) // viewProjection用
		.AddDescriptorTable(spriteDescriptorRange, _countof(spriteDescriptorRange), D3D12_SHADER_VISIBILITY_PIXEL) // Texture用
		.AddCBV(1, D3D12_SHADER_VISIBILITY_PIXEL)  // directionalLight用
		.AddCBV(2, D3D12_SHADER_VISIBILITY_PIXEL)  // pointLight用
		.AddCBV(3, D3D12_SHADER_VISIBILITY_PIXEL)  // spotLight用
		.AddSampler(CreateSampler(D3D12_TEXTURE_ADDRESS_MODE_WRAP))
		.Build(device_);
}



