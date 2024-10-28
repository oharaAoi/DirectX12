#include "WaterSpace.hlsli"

struct Material {
	float4 color;
	int enableLighting;
	float4x4 uvTransform;
	float shininess; // 光沢度
};

struct DirectionalLight {
	float4 color;
	float3 direction;
	float3 eyePos; // 視点の位置
	float intensity;
	float limPower;
};

struct PointLight {
	float4 color; // ライトの色
	float3 position; // ライトの位置
	float3 eyePos;
	float intensity; // 輝度
	float radius; // 最大距離
	float decay; // 減衰率
};

struct SpotLight {
	float4 color; // ライトの色
	float3 position; // ライトの位置
	float3 eyePos; // 視点の位置
	float intensity; // 輝度
	float3 direction; // 方向
	float distance; // ライトの届く最大距離
	float decay; // 減衰率
	float cosAngle; // スポットライトの余弦
	float cosFalloffStart;
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<PointLight> gPointLight : register(b2);
ConstantBuffer<SpotLight> gSpotLight : register(b3);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);
struct PixelShaderOutput {
	float4 color : SV_TARGET0;
};

//==========================================
// Lambert
//==========================================
float3 Lambert(float NdotL, float3 textureColor) {
	float3 resultColor = textureColor * NdotL;
	return resultColor;
}

//==========================================
// HalfLambert
//==========================================
float3 HalfLambert(float NdotL, float3 lightColor) {
	float cos = (pow(NdotL * 0.5f + 0.5f, 2.0f)) / 3.1415f;
	float3 diffuse = lightColor * cos;
	
	return diffuse;
}

//==========================================
//　phong
//==========================================
float3 Phong(float RDotE, float3 lightColor) {
	// 反射強度
	float specularPow = pow(saturate(RDotE), gMaterial.shininess);
	// 鏡面反射
	float3 specular = lightColor.rbg * specularPow * float3(1.0f, 1.0f, 1.0f);

	return specular;
}

//==========================================
//　BlinnPhong
//==========================================
float3 BlinnPhong(float NDotH, float3 lightColor) {
	// 反射強度
	float specularPow = pow(saturate(NDotH), gMaterial.shininess);
	// 鏡面反射
	float3 specular = lightColor.rbg * specularPow * float3(1.0f, 1.0f, 1.0f);

	return specular;
}

//==========================================
//　main
//==========================================
PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
	
	if (gMaterial.enableLighting == 0) {
		output.color = gMaterial.color * textureColor;
		return output;
	}
	
	float3 pointLightDirection = normalize(input.worldPos.xyz - gPointLight.position);
	float3 toEye = normalize(gDirectionalLight.eyePos - input.worldPos.xyz);
	float3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
	float3 halfVector = normalize(-gDirectionalLight.direction + toEye);
	
	float3 normal = normalize(input.normal);
	float3 lightDire = normalize(gDirectionalLight.direction);
	
	float RdotE = dot(reflectLight, toEye);
	float NdotH = dot(normalize(input.normal), halfVector);
	float NdotL = dot(normalize(input.normal), normalize(-gDirectionalLight.direction));
	
	float distance = length(gPointLight.position - input.worldPos.xyz);
	float factor = pow(saturate(-distance / gPointLight.radius + 1.0f), gPointLight.decay);
	
	if (textureColor.a <= 0.5f) {
		discard;
	}
	
	// --------------------- directional --------------------- //
	// lambert
	float3 directionalDiffuse;
	if (gMaterial.enableLighting == 1) {
		directionalDiffuse = Lambert(NdotL, gDirectionalLight.color.rgb) * gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.intensity;
	}
	else {
		directionalDiffuse = HalfLambert(NdotL, gDirectionalLight.color.rgb) * gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.intensity;
	}
	
	// phong
	float3 directionalSpeculer = BlinnPhong(NdotH, gDirectionalLight.color.rgb) * textureColor.rgb * gDirectionalLight.intensity;
	
	// --------------------- point --------------------- //
	float3 pointDiffuse;
	if (gMaterial.enableLighting == 1) {
		pointDiffuse = HalfLambert(NdotL, gPointLight.color.rgb * factor) * gMaterial.color.rgb * textureColor.rgb * gPointLight.intensity;
	}
	else {
		pointDiffuse = Lambert(NdotL, gPointLight.color.rgb * factor) * gMaterial.color.rgb * textureColor.rgb * gPointLight.intensity;
	}
	// phong
	float3 phalfVector = normalize(-pointLightDirection + toEye);
	float pNdotH = dot(normalize(input.normal), phalfVector);
	float3 pointSpeculer = BlinnPhong(pNdotH, gPointLight.color.rgb * factor) * textureColor.rgb * gPointLight.intensity;
	
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
	float3 spotColor = gSpotLight.color.rgb * gSpotLight.intensity * attenuationFactor * falloffFactor;
	
	// lambert
	float3 spotDiffuse;
	spotDiffuse = HalfLambert(NdotL, spotColor.rgb) * gMaterial.color.rgb * textureColor.rgb;
	
	// phong
	float3 spotSpeculer = BlinnPhong(NdotH, spotColor.rgb) * textureColor.rgb;
	
	// --------------------- limLight --------------------- //
	float lim = 1.0f - saturate(dot(normal, toEye));
	// リムライトの光の調整
	lim *= saturate(1.0f - saturate(dot(normal, lightDire)) + dot(toEye, lightDire));
	float3 limCol = pow(lim, gDirectionalLight.limPower) * gDirectionalLight.color.rgb * textureColor.rgb * gDirectionalLight.intensity;
	
	// --------------------- final --------------------- //
	output.color.rgb = directionalDiffuse + directionalSpeculer;
	//output.color.rgb += pointDiffuse + pointSpeculer;
	//output.color.rgb += spotDiffuse + spotSpeculer;
	//output.color.rgb += limCol;
	
	output.color.a = gMaterial.color.a * textureColor.a;
	
	if (output.color.a <= 0.1f) {
		discard;
	}
	
	return output;
}