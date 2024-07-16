
SamplerState gSampler : register(s0);
Texture2D<float4> gTexture : register(t0);
// 出力データにアクセスするための変数
RWTexture2D<float4> outputBuffer : register(u0);

[numthreads(16,16,1)]
void CSmain(uint3 id : SV_DispatchThreadID){
	uint2 pixcelCoordinate = id.xy;
	
	uint width, height;
	gTexture.GetDimensions(width, height);
	
	float2 uv = float2(pixcelCoordinate) / float2(width, height);

	//　textureからカラーをサンプリング
	float4 textureColor = gTexture.Sample(gSampler, uv);

	// モノクロ化の計算
	float gray = dot(textureColor.rgb, float3(0.299, 0.587, 0.114));

	// 出力バッファに結果を書き込む
	outputBuffer[pixcelCoordinate] = float4(gray, gray, gray, textureColor.a);
}