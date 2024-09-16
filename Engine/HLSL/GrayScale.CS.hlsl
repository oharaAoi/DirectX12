// グレースケールの係数
struct GrayScaleData{
	float GrayScaleAmount;
};

SamplerState gSampler : register(s0);
Texture2D<float4> gTexture : register(t0);
// 出力データにアクセスするための変数
RWTexture2D<float4> outputBuffer : register(u0);
// CPUからのデータを受け取るバッファ
ConstantBuffer<GrayScaleData> gGrayScale : register(b0);

[numthreads(16,16,1)]
void CSmain(uint3 id : SV_DispatchThreadID){
	uint2 pixelCoordinate = id.xy;
	
	uint width, height;
	gTexture.GetDimensions(width, height);
	
	float2 uv = float2(pixelCoordinate) / float2(width, height);

	//　textureからカラーをサンプリング
	float4 textureColor = gTexture.Load(int3(pixelCoordinate, 0));

	// モノクロ化の計算
	float gray = dot(textureColor.rgb, float3(0.299, 0.587, 0.114));
	
	// パラメータでgray値を変更
	float3 blendColor = lerp(textureColor.rgb, float3(gray, gray, gray), gGrayScale.GrayScaleAmount);

	// 出力バッファに結果を書き込む
	outputBuffer[pixelCoordinate] = float4(blendColor, textureColor.a);
}