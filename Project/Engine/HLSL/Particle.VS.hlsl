#include "Particle.hlsli"

struct ParticleForGPU{
	float4x4 world;
	float4x4 view;
	float4x4 projection;
	float4x4 worldInverseTranspose;
	float4 color;
};

//ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);
StructuredBuffer<ParticleForGPU> gParticleForGPU : register(t0);
struct VertexShaderInput{
	float4 position : POSITION0;
	float2 texcoord : TEXCOORD0;
	float4 colot : COLOR0;
};

VertexShaderOutput main(VertexShaderInput input, uint instanceId : SV_InstanceID){
	VertexShaderOutput output;
	// WVPの生成
	float4x4 WVP = mul(gParticleForGPU[instanceId].world, mul(gParticleForGPU[instanceId].view, gParticleForGPU[instanceId].projection));
	output.position = mul(input.position, WVP);
	output.texcoord = input.texcoord;
	output.color = gParticleForGPU[instanceId].color;
	return output;
}