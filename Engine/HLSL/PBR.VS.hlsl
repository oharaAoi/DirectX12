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
	output.normal = normalize(mul(input.normal, (float3x3) gWorldTransformMatrix.world));
	output.worldPos = mul(input.position, gWorldTransformMatrix.world);
	
	// 基底ベクトル
	float3 normal = normalize(mul(input.normal, (float3x3) gWorldTransformMatrix.world));
	float3 tangent = normalize(mul(input.normal, (float3x3) gWorldTransformMatrix.world));
	float3 bTangent = normalize(cross(normal, tangent));
	
	output.tangentMat = float3x3(tangent, bTangent, normal);
	
	
	return output;
}