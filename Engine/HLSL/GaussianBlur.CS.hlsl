
struct GaussianBlurData{
	
};

SamplerState gSampler : register(s0);
Texture2D<float4> gTexture : register(t0);
// 出力データにアクセスするための変数
RWTexture2D<float4> outputBuffer : register(u0);
// CPUからのデータを受け取るバッファ
ConstantBuffer<GaussianBlurData> gGaussianBlur : register(b0);

[numthreads(16, 16, 1)]
void CSmain(uint3 id : SV_DispatchThreadID)
{
	uint2 pixcelCoordinate = id.xy;
	
	uint width, height;
	gTexture.GetDimensions(width, height);
	
	float2 uv = float2(pixcelCoordinate) / float2(width, height);

	//　textureからカラーをサンプリング
	float4 textureColor = gTexture.Sample(gSampler, uv);

	// 出力バッファに結果を書き込む
	outputBuffer[pixcelCoordinate] = textureColor;

}