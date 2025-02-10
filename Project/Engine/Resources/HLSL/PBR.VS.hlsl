#include "PBR.hlsli"

struct WorldTransformMatrix
{
	float4x4 world;
	float4x4 worldInverseTranspose;
};

struct ViewProjectionMatrix
{
	float4x4 view;
	float4x4 projection;
};

ConstantBuffer<WorldTransformMatrix> gWorldTransformMatrix : register(b0);
ConstantBuffer<ViewProjectionMatrix> gViewProjectionMatrix : register(b1);

struct VertexShaderInput{
	float4 position : POSITION0;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
	float4 worldPos : WORLDPOS0;
	float3 tangent : TANGENT;
};

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;
	// WVPの生成
	float4x4 WVP = mul(gWorldTransformMatrix.world, mul(gViewProjectionMatrix.view, gViewProjectionMatrix.projection));
	output.position = mul(input.position, WVP);
	output.texcoord = input.texcoord;
	output.normal = normalize(mul(input.normal, (float3x3) gWorldTransformMatrix.worldInverseTranspose));
	output.worldPos = mul(input.position, gWorldTransformMatrix.world);
	float3 tangent = input.tangent;
	float3 normal = input.normal;
	float3 bitangent = cross(normal, tangent);
	float3x3 TBN = float3x3(tangent, bitangent, normal);
	output.tangentMat = TBN;
	
	return output;
}