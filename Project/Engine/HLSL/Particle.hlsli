
struct VertexShaderOutput{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float4 color : COLOR0;
};

struct Particle {
	float4 color;
	float3 scale;
	float3 translate;
	float3 velocity;
	float3 acceleration;
	float lifeTime;
	float currentTime;
	float damping;
	float gravity;
};