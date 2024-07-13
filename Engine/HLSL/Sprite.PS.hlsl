#include "Sprite.2d.hlsli"

struct Material{
	float4 color;
};

ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);
struct PixelShaderOutput{
	float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input){
	PixelShaderOutput output;
	float4 textureColor = gTexture.Sample(gSampler, input.texcoord);
	
	float Y = 0.299f * textureColor.r + 0.587f * textureColor.g + 0.114f * textureColor.b;
	
	
	
	
	output.color.r =  Y;
	output.color.g =  Y;
	output.color.b =  Y;
	output.color.a =  1.0f;
	
	return output;
}