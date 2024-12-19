#include "Primitive.hlsli"

struct TransformationMatrix{
	float4x4 WVP;
};

StructuredBuffer<TransformationMatrix> gTransformationMatrix : register(t0);
struct VertexSahderInput{
	float4 position : POSITION0;
	float4 color : COLOR0;
};

VertexShaderOutput main(VertexSahderInput input, uint instanceId : SV_InstanceID) {
	VertexShaderOutput output;
	output.position = mul(input.position, gTransformationMatrix[instanceId].WVP);
	output.color = input.color;
	return output;
}