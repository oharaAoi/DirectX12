
SamplerState gSampler : register(s0);
Texture2D<float4> gTexture1 : register(t0);
Texture2D<float4> gTexture2 : register(t1);
// 出力データにアクセスするための変数
RWTexture2D<float4> outputBuffer : register(u0);

[numthreads(16, 16, 1)]
void CSmain(uint3 id : SV_DispatchThreadID){
	uint2 pixcelCoordinate = id.xy;
	
	uint width, height;
	gTexture1.GetDimensions(width, height);
	float2 uv = float2(pixcelCoordinate) / float2(width, height);
	
	float4 finalColor = gTexture1.Sample(gSampler, uv) + gTexture2.Sample(gSampler, uv);

	outputBuffer[pixcelCoordinate] = finalColor;
}