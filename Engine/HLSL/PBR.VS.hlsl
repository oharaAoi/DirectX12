#include "PBR.hlsli"

struct TransformationMatrix{
	float4x4 world;
	float4x4 view;
	float4x4 projection;
	float4x4 worldInverseTranspose;
};

ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);

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
	float4x4 WVP = mul(gTransformationMatrix.world, mul(gTransformationMatrix.view, gTransformationMatrix.projection));
	output.position = mul(input.position, WVP);
	output.texcoord = input.texcoord;
	output.normal = normalize(mul(input.normal, (float3x3) gTransformationMatrix.world));
	output.worldPos = mul(input.position, gTransformationMatrix.world);
	
	// 基底ベクトル
	float3 normal = normalize(mul(input.normal, (float3x3)gTransformationMatrix.world));
	float3 tangent = normalize(mul(input.normal, (float3x3)gTransformationMatrix.world));
	float3 bTangent = normalize(cross(normal, tangent));
	
	output.tangentMat = float3x3(tangent, bTangent, normal);
	
	
	return output;
}