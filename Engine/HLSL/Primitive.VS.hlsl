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
	//float4x4 worldMat = float4x4(1.0f, 0.0f, 0.0f, 0.0f,
 //                                   0.0f, 1.0f, 0.0f, 0.0f,
 //                                   0.0f, 0.0f, 1.0f, 0.0f,
 //                                   input.position.x, input.position.y, input.position.z, 1.0f);
	
	//float4x4 wvpMat = mul(worldMat, gTransformationMatrix.WVP);
	
	//output.position = mul(input.position, wvpMat);
	output.position = mul(input.position, gTransformationMatrix.WVP);
	output.color = input.color;
	return output;
}