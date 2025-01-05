#include "Object3d.hlsli"

struct Material {
	float4x4 uvTransform;
};

ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput {
	float4 color : SV_TARGET0;
};

PixelShaderOutput main(TrailVertexShaderOutput input) {
	PixelShaderOutput output;
	
	float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
	
	output.color = input.color * textureColor;
	output.color = clamp(output.color, 0.0f, 1.0f);
	if (output.color.a == 0.0) {
		discard;
	}
	return output;
}