
struct DepthData {
	
};

SamplerState gSampler : register(s0);
Texture2D<float4> gTexture : register(t0);
// 出力データにアクセスするための変数
RWTexture2D<float4> outputBuffer : register(u0);
// CPUからのデータを受け取るバッファ
ConstantBuffer<DepthData> gDepthData : register(b0);

[numthreads(16, 16, 1)]
void CSmain(uint3 id : SV_DispatchThreadID) {
	uint2 pixelCoordinate = id.xy;
	float4 depthValue = gTexture.Load(int3(pixelCoordinate, 0));// 深度バッファから深度値を取得
	float4 color = depthValue;
	outputBuffer[pixelCoordinate] = color; // 深度値を出力
}