
#include "WaterSpace.hlsli"

struct WaveParameter {
	float amplitude;
	float frequency;
	float time;
};

struct WorldTransformMatrix {
	float4x4 world;
	float4x4 worldInverseTranspose;
};

struct ViewProjectionMatrix {
	float4x4 view;
	float4x4 projection;
};

ConstantBuffer<WorldTransformMatrix> gWorldTransformMatrix : register(b0);
ConstantBuffer<ViewProjectionMatrix> gViewProjectionMatrix : register(b1);
ConstantBuffer<WaveParameter> gWaveParameter : register(b2);

struct VertexShaderInput {
	float4 position : POSITION0;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
	float4 worldPos : WORLDPOS0;
	float3 tangent : TANGENT0;
};

float WaveHeight(float4 position) {
	return gWaveParameter.amplitude * sin(gWaveParameter.frequency * (position.x + gWaveParameter.time));
}

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;
	
	if (input.normal.y == 1.0f) {
		input.position.y += WaveHeight(input.position);
	}
	
	if (input.normal.x != 0.0f || input.normal.z != 0.0f) {
		input.position.y += WaveHeight(input.position);
	}
	
	// WVPの生成
	float4x4 WVP = mul(gWorldTransformMatrix.world, mul(gViewProjectionMatrix.view, gViewProjectionMatrix.projection));
	output.position = mul(input.position, WVP);
	output.texcoord = input.texcoord;
	output.normal = normalize(mul(input.normal, (float3x3) gWorldTransformMatrix.worldInverseTranspose));
	output.worldPos = mul(input.position, gWorldTransformMatrix.world);
	return output;
}