// サンプラーステート
SamplerState gSampler : register(s0);
// 3DオブジェクトのRender Target
Texture2D<float4> gRenderTarget3D : register(t0);
// UIのRender Target
Texture2D<float4> gRenderTargetUI : register(t1);
// 出力Render Targetにアクセスするための変数
RWTexture2D<float4> outputBuffer : register(u0);

[numthreads(16, 16, 1)]
void CSmain(uint3 id : SV_DispatchThreadID) {
	uint2 pixelCoordinate = id.xy;

    // 3Dオブジェクトの色を取得
	float4 color3D = gRenderTarget3D.Load(int3(pixelCoordinate, 0));
    // UIの色を取得
	float4 colorUI = gRenderTargetUI.Load(int3(pixelCoordinate, 0));

    // UIのアルファを使用して合成
    // 色が変わらないように、UIの色のアルファを使ってブレンド
	float4 resultColor = lerp(color3D, colorUI, colorUI.a);

    // 結果を出力バッファに書き込む
	outputBuffer[pixelCoordinate] = resultColor;
}