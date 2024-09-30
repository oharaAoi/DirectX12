#include "Particle.hlsli"

struct Material
{
	float4 color;
	int enableLighting;
	float4x4 uvTransform;
	
	// 反射の要素
	float aldedo;
	
	// 屈折率
	float refractiveIndex;
};

struct DirectionalLight
{
	float4 color;
	float3 direction;
	float3 eyePos; // 視点の位置
	float intensity;
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);
struct PixelShaderOutput
{
	float4 color : SV_TARGET0;
};

//==========================================
// Lambert
//==========================================
float4 Lambert(float NdotL, float4 textureColor)
{
	//float4 diffuse = gMaterial.diffuseColor + (1.0f / PI);
	float4 resultColor = gDirectionalLight.color * NdotL;
	return resultColor;
}

//==========================================
// HalfLambert
//==========================================
float4 HalfLambert(float NdotL)
{
	//float4 diffuseColor = gMaterial.diffuseColor + (1.0f / PI);
	float cos = (pow(NdotL * 0.5f + 0.5f, 2.0f));
	float4 diffuse = gDirectionalLight.color * cos;
	
	return diffuse;
}


PixelShaderOutput main(VertexShaderOutput input)
{
	PixelShaderOutput output;
	float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
	
	output.color.rgb = textureColor.rgb * gMaterial.color.rgb * input.color.rbg;
	output.color.a = textureColor.a * input.color.a;
	
	if (output.color.a <= 0.01f){
		discard;
	}
	
	return output;
}