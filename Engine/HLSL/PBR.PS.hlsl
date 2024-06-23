#include "PBR.hlsli"

// 定数定義
static const float PI = 3.141592653589f;
#define EPSILON 1e-6;

struct Material{
	float4 color;
	int enableLighting;
	float4x4 uvTransform;
	float4 diffuseColor; // 色
	float4 specularColor;
	float roughness; // 粗さ
	float metallic;// 金属度
	float shininess;// 鋭さ
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
Texture2D<float4> gNormapMap : register(t1);

SamplerState gSampler : register(s0);
//SamplerState gSampler2 : register(s1);
struct PixelShaderOutput{
	float4 color : SV_TARGET0;
};

//////////////////////////////////////////////////////////////
// 関数
//////////////////////////////////////////////////////////////
//==========================================
// Phong
//==========================================
float4 Phong(VertexShaderOutput input){
	float3 normal = normalize(input.normal);
	float3 viewDire = normalize(gDirectionalLight.eyePos - input.worldPos.xyz);
	float3 lightDire = -normalize(gDirectionalLight.direction);

	float NdotL = dot(normal, lightDire);
	//float3 reflection = reflect(lightDire, normal);
	float spec = 0;
	// ライトが表面に当たっていたら計算する
	if (NdotL > 0.0f){
		float3 reflection = normalize(2.0 * normal * NdotL - lightDire);
		spec = saturate(dot(reflection, viewDire));
		spec = pow(spec, gMaterial.shininess);
	}
	
	float4 specColor = spec * gMaterial.specularColor * gMaterial.metallic;
	return specColor;
}

float4 NormalizePhong(VertexShaderOutput input){
	float3 normal = normalize(input.normal);
	float3 lightDire = normalize(-gDirectionalLight.direction);
	float3 view = normalize(gDirectionalLight.eyePos - input.worldPos.xyz);
	float3 reflection = reflect(-lightDire, normal);
	// 
	float NdotL = saturate(dot(normal, lightDire));
	float LdotR = saturate(dot(lightDire, reflection));
	// 拡散色と鏡面反射色を求める
	float4 diffuse = gMaterial.color * (1.0f - gMaterial.metallic) * (1.0f / PI);
	float4 specular = gMaterial.color * gMaterial.metallic * ((gMaterial.shininess + 2.0f) / (2.0f * PI)) * pow(LdotR, gMaterial.shininess);
	// 最終的な色
	float4 result = (diffuse + specular) * gDirectionalLight.color;
	
	return result;
}

//float NdotL = (dot(normalize(input.normal), lightDire));
//float3 reflection = normalize(2.0f * normalize(input.normal) * NdotL - lightDire);

//==========================================
// Fresnel(Schlick)		F
//==========================================
float4 SchlickFresnel(float VDotH, float4 ks){
	float4 fresnel = (ks + (1.0f - ks) * pow((1.0f - VDotH), 5.0f));
	// textureColorfresnel = gMaterial.specularColor + (1.0f - gMaterial.specularColor) * pow((1.0f - VDotH), 5.0f);
	return fresnel;
}

//==========================================
// GGX					D
//==========================================
float GGX(float NDotH){
	// 粗さ
	float roughness = gMaterial.roughness * gMaterial.roughness + EPSILON;
	float denominator = (NDotH * NDotH) * (roughness - 1.0f) + 1.0f;
	
	return roughness / (PI * denominator * denominator);
}

//==========================================
// Height Correlated Smith		G
//==========================================
float HCSmith(float NdotV, float NdotL){
	float roughness = gMaterial.roughness * gMaterial.roughness + EPSILON;
	float NdotV2 = NdotV * NdotV;
	float NdotL2 = NdotL * NdotL;
	
	float Lambda_v = (-1.0 + sqrt(roughness * (1.0 - NdotL2) / NdotL2 + 1.0)) * 0.5f;
	float Lambda_l = (-1.0 + sqrt(roughness * (1.0 - NdotV2) / NdotV2 + 1.0)) * 0.5f;
	return 1.0 / (1.0 + Lambda_l + Lambda_v);
}

//==========================================
// BRDF(双方向反射率分布関数)
//==========================================
float4 BRDF(float NdotH, float NDotV, float NDotL, float VDotH, float4 ks){
	float D = GGX(NdotH);
	float G = HCSmith(NDotV, NDotL);
	float4 F = SchlickFresnel(VDotH, ks);
	
	float4 brdf = (D*G*F) / (4 * NDotV * NDotL);
	
	brdf = saturate(brdf);
	
	return brdf;
}

//////////////////////////////////////////////////////////////
// main
//////////////////////////////////////////////////////////////
PixelShaderOutput main(VertexShaderOutput input){
	PixelShaderOutput output;
	float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	
	float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
	float3 normalMap = gNormapMap.Sample(gSampler, transformedUV.xy).xyz * 2.0 - 1.0;
	
	float3 normal = normalize(input.normal);
	float3 lightDir = normalize(-gDirectionalLight.direction);
	
	float3 normal2 = mul(normalMap, input.tangentMat);
	
	//=======================================================
	// 色を求める
	//=======================================================
	float4 kd = gMaterial.color * (1.0 - gMaterial.metallic);
	float4 ks = gMaterial.color * gMaterial.metallic;
	float4 diffuse = kd / PI;
	
	//=======================================================
	// 内積などを求める
	//=======================================================
	float3 viewDir = normalize(gDirectionalLight.eyePos - input.worldPos.xyz);
	float3 halfVec = normalize(viewDir + (lightDir));
	float NdotH = saturate(dot(normal, halfVec));
	float NDotV = saturate(dot(normal, viewDir));
	float NDotL = saturate(dot(normal, lightDir));
	float VDotH = saturate(dot(viewDir, halfVec));
	
	//=======================================================
	// BRDF(双方向反射率分布関数)
	//=======================================================
	float4 brdf = BRDF(NdotH, NDotV, NDotL,VDotH, ks);
	
	//=======================================================
	 // 反射と拡散のバランスを取る
	float4 finalColor = brdf + diffuse;
	
	// レンダリング方程式の適用
	finalColor = finalColor * gDirectionalLight.intensity * NDotL;
	
	// テクスチャの色やライトの色を適応
	output.color = finalColor * textureColor * gDirectionalLight.color;
	
	output.color = finalColor * gDirectionalLight.color;
	
	//output.color = float4(normal2 * 0.5 + 0.5, 1.0f);
	
	return output;
}