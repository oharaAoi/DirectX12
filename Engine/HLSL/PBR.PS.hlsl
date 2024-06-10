#include "PBR.hlsli"

struct Material{
	float4 color;
	int enableLighting;
	float4x4 uvTransform;
	// 反射の要素
	float aldedo;
	// 屈折率
	float refractiveIndex;
};

struct DirectionalLight{
	float4 color;
	float3 direction;
	float3 eyePos;
	float intensity;
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
Texture2D<float4> gTexture : register(t0);

SamplerState gSampler : register(s0);
struct PixelShaderOutput{
	float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input){
	PixelShaderOutput output;
	output.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	return output;
}