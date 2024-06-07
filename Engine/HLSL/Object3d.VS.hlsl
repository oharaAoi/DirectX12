#include "Object3d.hlsli"

struct TransformationMatrix{
	float4x4 world;
	float4x4 view;
	float4x4 projection;
};

ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);

struct VertexShaderInput{
	float4 position : POSITION0;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
	float4 worldPos : WORLDPOS0;
};

VertexShaderOutput main(VertexShaderInput input){
	VertexShaderOutput output;
	// WVPの生成
	float4x4 WVP = mul(gTransformationMatrix.world, mul(gTransformationMatrix.view, gTransformationMatrix.projection));
	output.position = mul(input.position, WVP);
	output.texcoord = input.texcoord;
	output.normal = normalize(mul(input.normal, (float3x3)gTransformationMatrix.world));
	output.worldPos = mul(input.position, gTransformationMatrix.world);
	return output;
}