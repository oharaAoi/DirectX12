
SamplerState gSampler : register(s0);
Texture2D<float4> gTexture : register(t0);
// 出力データにアクセスするための変数
RWTexture2D<float4> outputBuffer : register(u0);

[numthreads(16, 16, 1)]
void CSmain(uint3 id : SV_DispatchThreadID){
	uint2 pixelCoordinate = id.xy;
	
	float4 textureColor = gTexture.Load(int3(pixelCoordinate, 0));

	outputBuffer[pixelCoordinate] = textureColor;
}

// -------------------------------------------------
// ↓ Textureから高さと横幅を取得する
// -------------------------------------------------
//uint width, height;
//	gTexture.GetDimensions(width, height);
//float2 uv = float2(pixelCoordinate) / float2(width, height);
//float4 textureColor = gTexture.Sample(gSampler, uv);