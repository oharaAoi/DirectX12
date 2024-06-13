#include "PBR.hlsli"

// 定数定義
static const float PI = 3.1415926f;

struct Material{
	float4 color;
	int enableLighting;
	float4x4 uvTransform;
	// 反射の要素
	float aldedo;
	// 屈折率
	float refractiveIndex;
	// 反射率
	float refraction;
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

//////////////////////////////////////////////////////////////
// 関数
//////////////////////////////////////////////////////////////

//==========================================
// Lambert
//==========================================
float4 Lambert(VertexShaderOutput input){
	float NdotL = dot(normalize(input.normal), normalize(-gDirectionalLight.direction));
	float cos = (pow(NdotL * 0.5f + 0.5f, 2.0f) * gMaterial.aldedo) / PI;
	float4 diffuse = gDirectionalLight.color * cos;
	
	return diffuse;
}

//==========================================
// Phong
//==========================================
float4 Phong(VertexShaderOutput input){
	float3 viewDire = normalize(gDirectionalLight.eyePos - input.worldPos.xyz);
	float3 lightDire = normalize(gDirectionalLight.direction);

	float3 reflection = reflect(lightDire, normalize(input.normal));

	float spec = saturate(dot(reflection, viewDire));
	spec = pow(spec, 1.0f);
	
	float4 specColor = float4(1.0, 1.0, 1.0, 1.0f) * spec;
	return specColor;
}


//float NdotL = (dot(normalize(input.normal), lightDire));
//float3 reflection = normalize(2.0f * normalize(input.normal) * NdotL - lightDire);

//==========================================
// Fresnel(Schlick)
//==========================================
float4 SchlickFresnel(VertexShaderOutput input){
	float3 halfVector = normalize(-gDirectionalLight.direction + gDirectionalLight.eyePos);
	float cos = dot(normalize(input.normal), halfVector);
	float fresnel = gMaterial.refraction + (1.0f - gMaterial.refraction) * pow((1.0f - cos), 5.0f);
	
	return float4(1.0f, 1.0f, 1.0f, 1.0f) * fresnel;
}
	

//////////////////////////////////////////////////////////////
// main
//////////////////////////////////////////////////////////////
PixelShaderOutput main(VertexShaderOutput input){
	PixelShaderOutput output;
	float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
	
	//=======================================================
	// Lambert反射
	//=======================================================
	float4 lambertColor;
	if (gMaterial.enableLighting != 0){
		lambertColor = Lambert(input);
	}else{
		output.color = gMaterial.color * textureColor;
	}
	
	//=======================================================
	// Phong反射
	//=======================================================
	float4 specularColor = Phong(input);	
	
	//=======================================================
	// フレネル反射
	//=======================================================
	float4 fresnel = SchlickFresnel(input);
	
	//=======================================================
	// 最終
	//=======================================================
	float4 refrectionColor = lambertColor + specularColor + fresnel;
	
	output.color = refrectionColor * textureColor * gMaterial.color * gDirectionalLight.intensity;
	//output.color = refrectionColor * gDirectionalLight.intensity;
	
	return output;
}