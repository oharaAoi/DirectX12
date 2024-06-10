#include "Object3d.hlsli"

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
	float3 eyePos; // 視点の位置
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
	float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
	
	// lambert
	float4 lambertColor;
	
	if (gMaterial.enableLighting != 0){
		float NdotL = dot(normalize(input.normal), normalize(-gDirectionalLight.direction));
		float cos = (pow(NdotL * 0.5f + 0.5f, 2.0f) * gMaterial.aldedo) / 3.1415f;
		lambertColor = gDirectionalLight.color * cos;
	}else{
		output.color = gMaterial.color * textureColor;
	}
	
	// -------------------------------------------------
	// フレネル反射
	// 入射角度を求める
	// カメラ方向を取得
	float3 viewDir = normalize(gDirectionalLight.eyePos - (float3)input.worldPos);
	float incidence = dot(normalize(input.normal), viewDir);
	
	// 屈折角度を求める
	float sinIncidence = sqrt(1.0 - (incidence * incidence));
	float sinRefractionAngle = sinIncidence / gMaterial.refractiveIndex;
	float refractionAngle = asin(sinRefractionAngle * sinRefractionAngle);
	
	// フレネル反射の方程式で反射係数を算出
	float cosIncidence = cos(incidence);
	float cosRefractionAngle = cos(refractionAngle);
	
	// 平行偏光
	float rParallel = (gMaterial.refractiveIndex * cosIncidence - gMaterial.refractiveIndex * cosRefractionAngle) /
					(gMaterial.refractiveIndex * cosIncidence + gMaterial.refractiveIndex * cosRefractionAngle);

	// 垂直偏光
	float rPerpendicular = (cosIncidence - gMaterial.refractiveIndex * cosRefractionAngle) /
					(cosIncidence + gMaterial.refractiveIndex * cosRefractionAngle);

	float fresnelReflection = 0.5f * (rParallel * rParallel + rPerpendicular * rPerpendicular);
	
	// フレネル反射の方程式を使って反射光を算出する
	//float fresnelRefraction = normalize(pow(
	//(1.0f * cos(incidence))- (gMaterial.refractiveIndex * cos(refractionAngle) 
	/// 1.0f * cos(incidence)) + (gMaterial.refractiveIndex * cos(refractionAngle)), 2.0f));
	
	// カラーを足す
	float4 fresnelColor = gDirectionalLight.color * fresnelReflection;
	output.color = (fresnelColor) * textureColor * gMaterial.color * gDirectionalLight.intensity;
	// lambertColor
	
	return output;
}