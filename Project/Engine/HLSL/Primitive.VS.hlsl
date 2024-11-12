#include "Primitive.hlsli"

struct TransformationMatrix{
	float4x4 WVP;
};

ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);
struct VertexSahderInput{
	float4 position : POSITION0;
	float4 color : COLOR0;
};

VertexShaderOutput main(VertexSahderInput input){
	VertexShaderOutput output;
	output.position = mul(input.position, gTransformationMatrix.WVP);
	output.color = input.color;
	return output;
}