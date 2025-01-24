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


ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);
struct PixelShaderOutput{
	float4 color : SV_TARGET0;
};


PixelShaderOutput main(VertexShaderOutput input)
{
	PixelShaderOutput output;
	float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
	
	if (textureColor.a == 0.0) {
		discard;
	}
	
	output.color.rgb = textureColor.rgb * gMaterial.color.rgb * input.color.rgb;
	output.color.a = textureColor.a * input.color.a * gMaterial.color.a;
	
	//if (output.color.a == 0.0){
	//	discard;
	//}
	
	return output;
}