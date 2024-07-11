#include "Object3d.hlsli"

struct Material{
	float4 color;
	int enableLighting;
	float4x4 uvTransform;
	float shininess;		// 光沢度
};

struct DirectionalLight{
	float4 color;
	float3 direction;
	float3 eyePos; // 視点の位置
	float intensity;
};

struct PointLight{
	float4 color; // ライトの色
	float3 position; // ライトの位置
	float3 eyePos;
	float intensity; // 輝度
	float radius;	// 最大距離
	float decay;	// 減衰率
};

struct SpotLight{
	float4 color;		// ライトの色
	float3 position;	// ライトの位置
	float3 eyePos;		// 視点の位置
	float intensity;	// 輝度
	float3 direction;	// 方向
	float distance;		// ライトの届く最大距離
	float decay;		// 減衰率
	float cosAngle;		// スポットライトの余弦
	float cosFalloffStart;
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<PointLight> gPointLight : register(b2);
ConstantBuffer<SpotLight> gSpotLight : register(b3);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);
struct PixelShaderOutput{
	float4 color : SV_TARGET0;
};

//==========================================
// Lambert
//==========================================
float4 Lambert(float NdotL, float4 textureColor, float4 lightColor){
	//float4 diffuse = gMaterial.diffuseColor + (1.0f / PI);
	float4 resultColor = lightColor * NdotL;
	return resultColor;
}

//==========================================
// HalfLambert
//==========================================
float4 HalfLambert(float NdotL, float4 lightColor){
	float cos = (pow(NdotL * 0.5f + 0.5f, 2.0f)) / 3.1415f;
	float4 diffuse = lightColor * cos;
	
	return diffuse;
}

//==========================================
//　phong
//==========================================
float3 Phong(float RDotE, float4 lightColor){
	// 反射強度
	float specularPow = pow(saturate(RDotE), gMaterial.shininess);
	// 鏡面反射
	float3 specular = lightColor.rbg * specularPow * float3(1.0f, 1.0f, 1.0f);

	return specular;
}

//==========================================
//　BlinnPhong
//==========================================
float3 BlinnPhong(float NDotH, float4 lightColor){
	// 反射強度
	float specularPow = pow(saturate(NDotH), gMaterial.shininess);
	// 鏡面反射
	float3 specular = lightColor.rbg * specularPow * float3(1.0f, 1.0f, 1.0f);

	return specular;
}

//==========================================
//　main
//==========================================
PixelShaderOutput main(VertexShaderOutput input){
	PixelShaderOutput output;
	float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
	
	float3 pointLightDirection = normalize(input.worldPos.xyz - gPointLight.position);
	float3 toEye = normalize(gDirectionalLight.eyePos - input.worldPos.xyz);
	float3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
	float3 halfVector = normalize(-gDirectionalLight.direction + toEye);
	
	float RdotE = dot(reflectLight, toEye);
	float NdotH = dot(normalize(input.normal), halfVector);
	
	float distance = length(gPointLight.position - input.worldPos.xyz);
	float factor = pow(saturate(-distance / gPointLight.radius + 1.0f), gPointLight.decay);
	
	if (textureColor.a <= 0.5f){
		discard;
	}
	
	// --------------------- directional --------------------- //
	// lambert
	float3 directionalDiffuse;
	if (gMaterial.enableLighting != 0){
		float NdotL = dot(normalize(input.normal), normalize(-gDirectionalLight.direction));
		directionalDiffuse = HalfLambert(NdotL, gDirectionalLight.color).rbg * gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.intensity;
	} else{
		output.color = gMaterial.color * textureColor;
	}
	
	// phong
	float3 directionalSpeculer = BlinnPhong(NdotH, gDirectionalLight.color) * gDirectionalLight.intensity;
	
	// --------------------- point --------------------- //
	float3 pointDiffuse;
	if (gMaterial.enableLighting != 0){
		float NdotL = dot(normalize(input.normal), normalize(-pointLightDirection));
		pointDiffuse = HalfLambert(NdotL, gPointLight.color * factor).rbg * gMaterial.color.rgb * textureColor.rgb * gPointLight.intensity;
	} else {
		output.color = gMaterial.color * textureColor;
	}

	// phong
	float3 phalfVector = normalize(-pointLightDirection + toEye);
	float pNdotH = dot(normalize(input.normal), phalfVector);
	float3 pointSpeculer = BlinnPhong(pNdotH, gPointLight.color * factor) * gPointLight.intensity;
	
	// --------------------- spot --------------------- //
	// 入射光
	float3 spotLightDirectionOnSurface = normalize(input.worldPos.xyz - gSpotLight.position);
	// Falloff
	float cosAngle = dot(spotLightDirectionOnSurface, gSpotLight.direction);
	float falloffFactor = saturate((cosAngle - gSpotLight.cosAngle) / (gSpotLight.cosFalloffStart - gSpotLight.cosAngle));
	// 距離による減衰
	float distanceSpot = length(gSpotLight.position - input.worldPos.xyz);
	float attenuationFactor = pow(saturate(-distanceSpot / gSpotLight.distance + 1.0f), gSpotLight.decay);
	// スポットライトのカラー
	float4 spotColor = gSpotLight.color * gSpotLight.intensity * attenuationFactor * falloffFactor;
	
	// lambert
	float3 spotDiffuse;
	if (gMaterial.enableLighting != 0){
		float NdotL = dot(normalize(input.normal), normalize(-gSpotLight.direction));
		spotDiffuse = HalfLambert(NdotL, spotColor).rbg * gMaterial.color.rgb * textureColor.rgb;
		
	}else{
		output.color = gMaterial.color * textureColor;
	}
	
	// phong
	float3 spotSpeculer = BlinnPhong(NdotH, spotColor);
	
	// --------------------- limLight --------------------- //
	// サーフェイスの法線と光の入射方向によるリムの強さを計算
	float powerLimSurface = 1.0f - max(0.0f, dot(gDirectionalLight.direction, input.normal));
	// 視線によるリムの強さの計算
	float powwerLimView = 1.0f - max(0.0f, dot(toEye, input.normal));
	// 最終的なリムの強さを求める
	float limPower = powerLimSurface * powwerLimView;
	// リムライトのカラーを計算する
	float3 limColor = limPower * gDirectionalLight.color.rgb * gDirectionalLight.intensity;
	
	// --------------------- final --------------------- //
	output.color.rgb = directionalDiffuse + directionalSpeculer + pointDiffuse + pointSpeculer + spotDiffuse + spotSpeculer + limPower;
	output.color.a = gMaterial.color.a * textureColor.a;
	
	if (output.color.a <= 0.0f){
		discard;
	}
	
	return output;
}