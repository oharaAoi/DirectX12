
SamplerState gSampler : register(s0);
Texture2D<float4> gTexture : register(t0);
// 出力データにアクセスするための変数
RWTexture2D<float4> outputBuffer : register(u0);

[numthreads(16,16,1)]
void CSmain(uint3 id : SV_DispatchThreadID){
	uint2 pixcelCoordinate = id.xy;
	
	float2 uv = float2(pixcelCoordinate) / float2(1280, 720);

	//　textureからカラーをサンプリング
	float4 textureColor = gTexture.Sample(gSampler, uv);

	// モノクロ化の計算
	float gray = dot(textureColor.rgb, float3(0.299, 0.587, 0.114));
	
	float Y = 0.299f * textureColor.r + 0.587f * textureColor.g + 0.114f * textureColor.b;

	// 出力バッファに結果を書き込む
	outputBuffer[pixcelCoordinate] = float4(Y, Y, Y, textureColor.a);
}