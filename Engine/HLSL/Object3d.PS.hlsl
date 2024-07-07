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

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);
struct PixelShaderOutput{
	float4 color : SV_TARGET0;
};

//==========================================
// Lambert
//==========================================
float4 Lambert(float NdotL, float4 textureColor)
{
	//float4 diffuse = gMaterial.diffuseColor + (1.0f / PI);
	float4 resultColor = gDirectionalLight.color * NdotL;
	return resultColor;
}

//==========================================
// HalfLambert
//==========================================
float4 HalfLambert(float NdotL)
{
	//float4 diffuseColor = gMaterial.diffuseColor + (1.0f / PI);
	float cos = (pow(NdotL * 0.5f + 0.5f, 2.0f));
	float4 diffuse = gDirectionalLight.color * cos;
	
	return diffuse;
}

//==========================================
//　phong
//==========================================
float3 Phong(float RDotE){
	// 反射強度
	float specularPow = pow(saturate(RDotE), gMaterial.shininess);
	// 鏡面反射
	float3 specular = gDirectionalLight.color.rbg * gDirectionalLight.intensity * specularPow * float3(1.0f, 1.0f, 1.0f);

	return specular;
}

float3 BlinnPhong(float NDotH){
	// 反射強度
	float specularPow = pow(saturate(NDotH), gMaterial.shininess);
	// 鏡面反射
	float3 specular = gDirectionalLight.color.rbg * gDirectionalLight.intensity * specularPow * float3(1.0f, 1.0f, 1.0f);

	return specular;
}

PixelShaderOutput main(VertexShaderOutput input){
	PixelShaderOutput output;
	float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
	
	if (textureColor.a <= 0.5f){
		discard;
	}
	
	// lambert
	float3 lambertColor;
	if (gMaterial.enableLighting != 0){
		float NdotL = dot(normalize(input.normal), normalize(-gDirectionalLight.direction));
		float cos = (pow(NdotL * 0.5f + 0.5f, 2.0f)) / 3.1415f;
		lambertColor = gDirectionalLight.color.rgb * cos * gMaterial.color.rgb * textureColor.rgb;
	}else{
		output.color = gMaterial.color * textureColor;
	}
	
	// phong
	float3 toEye = normalize(gDirectionalLight.eyePos - input.worldPos.xyz);
	float3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
	float3 halfVector = normalize(-gDirectionalLight.direction + toEye);
	float RDotE = dot(reflectLight, toEye);
	float NDotH = dot(normalize(input.normal), halfVector);
	float3 speculer = BlinnPhong(NDotH);
	
	output.color.rgb = lambertColor + speculer;
	output.color.a = gMaterial.color.a * textureColor.a;
	
	if (output.color.a <= 0.0f){
		discard;
	}
	
	return output;
}