#include "Object3d.hlsli"

struct Material{
	float4 color;
	uint enableLighting;
	float4x4 uvTransform;
};

struct DirectionalLight{
	float4 color;
	float3 direction;
	float intensity;
};

struct PixelShaderOutput{
	float4 color : SV_TARGET0;
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
PixelShaderOutput main(VertexShaderOutput input){
	PixelShaderOutput output;
    
	if (gMaterial.enableLighting != 0){
		float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
		//float cos = saturate(dot(normalize(input.normal), -gDirectionalLight.direction));
		float cos = pow(NdotL * 0.5f + 0.5f, 2.0f) / 3.1415f;
		output.color = gMaterial.color * gDirectionalLight.color * cos * gDirectionalLight.intensity;
	}
	else{
		output.color = gMaterial.color;
	}
        
	return output;
}