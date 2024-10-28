#include "Object3d.hlsli"

struct Well {
	float4x4 skeletonSpaceMatrix;
	float4x4 skeletonSpaceInverseTransposeMatrix;
};

// 入力頂点
struct VertexShaderInput {
	float4 position : POSITION0;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
	float4 worldPos : WORLDPOS0;
	float3 tangent : TANGENT0;
	float4 weight : WEIGHT0;
	int4 index : INDEX0;
};

// 受け取り
struct Skinned {
	float4 position;
	float3 normal;
};

struct WorldTransformMatrix {
	float4x4 world;
	float4x4 worldInverseTranspose;
};

struct ViewProjectionMatrix {
	float4x4 view;
	float4x4 projection;
};

StructuredBuffer<Well> gMatrixPalette : register(t0);
ConstantBuffer<WorldTransformMatrix> gWorldTransformMatrix : register(b0);
ConstantBuffer<ViewProjectionMatrix> gViewProjectionMatrix : register(b1);

Skinned Skinning(VertexShaderInput input) {
	Skinned skinned;
	// 位置の変換
	skinned.position = mul(input.position, gMatrixPalette[input.index.x].skeletonSpaceMatrix) * input.weight.x;
	skinned.position += mul(input.position, gMatrixPalette[input.index.y].skeletonSpaceMatrix) * input.weight.y;
	skinned.position += mul(input.position, gMatrixPalette[input.index.z].skeletonSpaceMatrix) * input.weight.z;
	skinned.position += mul(input.position, gMatrixPalette[input.index.w].skeletonSpaceMatrix) * input.weight.w;
	skinned.position.w = 1.0f;
	
	// 法線の変換
	skinned.normal = mul(input.normal, (float3x3) gMatrixPalette[input.index.x].skeletonSpaceInverseTransposeMatrix) * input.weight.x;
	skinned.normal += mul(input.normal, (float3x3) gMatrixPalette[input.index.y].skeletonSpaceInverseTransposeMatrix) * input.weight.y;
	skinned.normal += mul(input.normal, (float3x3) gMatrixPalette[input.index.z].skeletonSpaceInverseTransposeMatrix) * input.weight.z;
	skinned.normal += mul(input.normal, (float3x3) gMatrixPalette[input.index.w].skeletonSpaceInverseTransposeMatrix) * input.weight.w;
	skinned.normal = normalize(skinned.normal);
	
	return skinned;
}

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;
	Skinned skinned = Skinning(input);
	float4x4 WVP = mul(gWorldTransformMatrix.world, mul(gViewProjectionMatrix.view, gViewProjectionMatrix.projection));
	output.position = mul(skinned.position, WVP);
	output.worldPos = mul(skinned.position, gWorldTransformMatrix.world);
	output.texcoord = input.texcoord;
	output.normal = normalize(mul(input.normal, (float3x3)gWorldTransformMatrix.worldInverseTranspose));
	
	return output;
}