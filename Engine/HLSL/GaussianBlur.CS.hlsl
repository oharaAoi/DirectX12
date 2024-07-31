
static const float PI = 3.141592653589f;

struct GaussianBlurData
{
	float blurStrength; // ぼかしの強さ
	float kernelSize; // カーネルサイズ
	float2 direction;
};

SamplerState gSampler : register(s0);
Texture2D<float4> gTexture : register(t0);
// 出力データにアクセスするための変数
RWTexture2D<float4> outputBuffer : register(u0);
// CPUからのデータを受け取るバッファ
ConstantBuffer<GaussianBlurData> gGaussianBlur : register(b0);

/// サンプルの重みづけ
float Gaussian(float x, float sigma)
{
	return exp(-(x * x) / (2.0 * sigma * sigma)) / (sqrt(2.0 * PI) * sigma);
}

[numthreads(16, 16, 1)]
void CSmain(uint3 id : SV_DispatchThreadID)
{
	uint2 pixelCoordinate = id.xy;
	
	// -------------------------------------------------
	// ↓ textureのサイズを取得する
	// -------------------------------------------------
	uint2 texSize;
	gTexture.GetDimensions(texSize.x, texSize.y);

}
