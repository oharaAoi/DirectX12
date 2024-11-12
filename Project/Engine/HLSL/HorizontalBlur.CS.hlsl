
static const float PI = 3.141592653589f;

struct GaussianBlurData {
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
float Gaussian(float x, float sigma) {
	return exp(-(x * x) / (2.0 * sigma * sigma)) / (sqrt(2.0 * PI) * sigma);
}

[numthreads(16, 16, 1)]
void CSmain(uint3 id : SV_DispatchThreadID) {
	uint2 pixelCoordinate = id.xy;
	
	// -------------------------------------------------
	// ↓ 強さや区分けするsizeが0以下の時はブラーを掛けない
	// -------------------------------------------------
	if (gGaussianBlur.blurStrength <= 0 || gGaussianBlur.kernelSize <= 0) {
		outputBuffer[pixelCoordinate] = gTexture.Load(int3(pixelCoordinate, 0));
		return;
	}
	
	// -------------------------------------------------
	// ↓ textureのサイズを取得する
	// -------------------------------------------------
	uint2 texSize;
	gTexture.GetDimensions(texSize.x, texSize.y);

	// -------------------------------------------------
	// ↓ 水平方向のブラーを実行する
	// -------------------------------------------------
	float4 resultColor = float4(0, 0, 0, 1);
	float totalWeight = 0.0f;
	
	float2 texelSize = 1.0 / float2(texSize); // テクセルのサイズ
	float kernelRadius = gGaussianBlur.kernelSize * 0.5f;
	
	for (int oi = -gGaussianBlur.kernelSize; oi < gGaussianBlur.kernelSize; ++oi) {
		float offset = oi * texelSize.x;
		float weight = Gaussian(offset, gGaussianBlur.blurStrength);
        
        // 境界処理
		uint2 sampleCoord = pixelCoordinate + uint2(oi, 0);
		sampleCoord = clamp(sampleCoord, uint2(0, 0), texSize - uint2(1, 1));
        
		resultColor += gTexture.Load(int3(sampleCoord, 0)) * weight;
		totalWeight += weight;
	}
	
	// 正規化
	resultColor /= totalWeight;
	
	// 出力バッファに結果を書き込む
	outputBuffer[pixelCoordinate] = resultColor;
}