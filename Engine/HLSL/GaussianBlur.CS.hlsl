
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
	uint2 pixcelCoordinate = id.xy;
	
	uint width, height;
	gTexture.GetDimensions(width, height);
	float2 uv = float2(pixcelCoordinate) / float2(width, height);
	
	if (gGaussianBlur.kernelSize == 0 || gGaussianBlur.blurStrength == 0){
		// 出力バッファに結果を書き込む
		outputBuffer[pixcelCoordinate] = gTexture.Sample(gSampler, uv);

	}else{
	
		float blurStrength = gGaussianBlur.blurStrength;
		float kernelSize = gGaussianBlur.kernelSize;
		float2 dire = normalize(gGaussianBlur.direction);
	
		float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float totalWeight = 0.0f;
		float sigma = kernelSize / 6.0f; // σの計算
	
		for (int i = -int(kernelSize) / 2; i <= int(kernelSize) / 2; i++){
			float2 offset = ((dire * float2(i, i)) / float2(width, height)) * blurStrength;
			float weight = Gaussian(float(i), sigma);
			color += gTexture.Sample(gSampler, uv + offset) * weight;
			totalWeight += weight;
		}
	
		// 正規化
		if (totalWeight > 0.0f){
			color /= totalWeight;
		}
	
		// 出力バッファに結果を書き込む
		outputBuffer[pixcelCoordinate] = color;
	}
}

//if (gGaussianBlur.direction == 0){
	//	for (int i = -int(kernelSize) / 2; i <= int(kernelSize) / 2; i++){
	//		float2 offset = float2(float(i), 0.0f) / float(width) * blurStrengthX;
	//		horizontalColor += gTexture.Sample(gSampler, uv + offset);
	//	}
	//	horizontalColor /= kernelSize;
		
	//	// 出力バッファに結果を書き込む
	//	outputBuffer[pixcelCoordinate] = horizontalColor;

	//}
	//else if (gGaussianBlur.direction == 1){
	//	for (int i = -int(kernelSize) / 2; i <= int(kernelSize) / 2; i++){
	//		float2 offset = float2(0.0f, float(i)) / float(height) * blurStrengthY;
	//		horizontalColor += gTexture.Sample(gSampler, uv + offset);
	//	}
	//	verticalColor /= kernelSize;
		
	//	// 出力バッファに結果を書き込む
	//	outputBuffer[pixcelCoordinate] = verticalColor;
	//}