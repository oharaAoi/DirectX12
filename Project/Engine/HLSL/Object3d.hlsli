struct VertexShaderOutput{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
	float4 worldPos : WORLDPOS0;
};

struct TrailVertexShaderOutput {
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float4 color	: COLOR0;
};