#include "Object3d.hlsli"

/// 定数バッファ
struct Material{
	float4 color;
	uint enableLighting;
	float4x4 uvTransform;
	uint lightingType;
};

struct DirectionLight{
	float4 lightColor; // ライトの色
	float3 lightDirection; // ライトの方向
	float3 eyePos; // 視点の位置
	float intensity; // 輝度 
};

struct PixelShaderOutput{
	float4 color : SV_TARGET0;
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionLight> gDirectionalLight : register(b1);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);
PixelShaderOutput main(VertexShaderOutput input)
{
	PixelShaderOutput output;
	float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
	float3 normal = normalize(input.normal);
	float RdotV = 0;
	
	// ======================================================
	// ピクセルの法線とライトの方向の内積を計算する
	// ======================================================
	float3 lightDir = -normalize(gDirectionalLight.lightDirection);
	float NdotL = saturate(dot(normal, lightDir));
	
	// ======================================================
	// 拡散反射光を求める
	// ======================================================
	float4 diffuseLight = gDirectionalLight.lightColor * NdotL;
	
	// ======================================================
	// 光が当たった場所からカメラに伸びるベクトルを求める
	// ======================================================
	float3 worldPos = input.worldPos.xyz;
	float3 toEye = normalize(gDirectionalLight.eyePos - worldPos);
	
	// ======================================================
	// 反射ベクトルを求める
	// ======================================================
	float3 refVec = reflect(lightDir, normal);
	//float3 refVec = normalize((2.0f * normal) * NdotL - lightDir);
	
	// ======================================================
	// 鏡面反射の強さを求める
	// ======================================================
	// refVecとtoEyeの内積を求める
	RdotV = saturate(dot(refVec, toEye));

	// ======================================================
	// 鏡面反射の強さを絞る
	// ======================================================
	RdotV = pow(RdotV, 1.0f);
	//float3 specColor = float3(1.0f, 1.0f, 1.0f) * RdotV;
	
	// ======================================================
	// 鏡面反射光を求める
	// ======================================================
	float4 specularLight = float4(1.0f, 1.0f, 1.0f, 1.0f) * RdotV;
	
	// ======================================================
	// 拡散反射光と鏡面反射光を足して、最終的な光を求める
	// ======================================================
	//float4 light = specularLight + diffuseLight;
	float4 light = specularLight/* + diffuseLight*/;
	
	// ======================================================
	// 環境光の実装
	// ======================================================
	light += 0.3f;
	
	// ======================================================
	// textureColorに求めた光を乗算して結果を求める
	// ======================================================
	output.color = textureColor * light * gMaterial.color * gDirectionalLight.intensity;
	
	return output;
}