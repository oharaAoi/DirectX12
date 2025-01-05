#include "Object3d.hlsli"

struct ViewProjectionMatrix {
	float4x4 view;
	float4x4 projection;
};

ConstantBuffer<ViewProjectionMatrix> gViewProjectionMatrix : register(b0);

struct VertexShaderInput {
	float4 position : POSITION0;
	float2 texcoord : TEXCOORD0;
	float4 color	: COLOR0;
};

TrailVertexShaderOutput main(VertexShaderInput input) {
	TrailVertexShaderOutput output;
	float4x4 VP = mul(gViewProjectionMatrix.view, gViewProjectionMatrix.projection);
	output.position = mul(input.position, VP);
	output.texcoord = input.texcoord;
	output.color = input.color;
	return output;

}