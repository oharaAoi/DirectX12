#include "Object3d.hlsli"

struct TimeCount {
	float time;
	float totalTime;
	
	float3 cameraPos;
};

struct WorldTransformMatrix {
	float4x4 world;
	float4x4 worldInverseTranspose;
};

struct ViewProjectionMatrix {
	float4x4 view;
	float4x4 projection;
};

float rand3dTo1d(float3 value, float3 dotDir = float3(12.9898, 78.233, 37.719)) {
	float3 smallValue = sin(value);
	float random = dot(smallValue, dotDir);
	random = frac(sin(random) * 143758.5453);
	return random;
}

float3 rand3dTo3d(float3 value) {
	return float3(
        rand3dTo1d(value, float3(12.989, 78.233, 37.719)),
        rand3dTo1d(value, float3(39.346, 11.135, 83.155)),
        rand3dTo1d(value, float3(73.156, 52.235, 09.151))
    );
}

class RandomGenerator {
	float3 seed;
	float3 Generated3d() {
		seed = rand3dTo3d(seed);
		return seed;
	}
	float Generated1d() {
		float result = rand3dTo1d(seed);
		seed.x = result;
		return result;
	}
};

ConstantBuffer<TimeCount> gTimeCount : register(b2);
ConstantBuffer<WorldTransformMatrix> gWorldTransformMatrix : register(b0);
ConstantBuffer<ViewProjectionMatrix> gViewProjectionMatrix : register(b1);

[maxvertexcount(3)]
void main(
	triangle VertexShaderOutput input[3],
	inout TriangleStream<VertexShaderOutput> triStream
) {
    // WVP行列の計算
	float4x4 WVP = mul(mul(gWorldTransformMatrix.world, gViewProjectionMatrix.view), gViewProjectionMatrix.projection);
    
    // 三角形の重心を計算
	float3 center = (input[0].position.xyz + input[1].position.xyz + input[2].position.xyz) / 3.0;

    // 三角形の法線を計算
	float3 edge1 = input[1].position.xyz - input[0].position.xyz;
	float3 edge2 = input[2].position.xyz - input[0].position.xyz;
	float3 normal = normalize(cross(edge1, edge2)); // 法線方向

    // ランダムジェネレータを初期化（法線方向にバラバラに動かす）
	RandomGenerator generator;
	generator.seed = gTimeCount.time;

    // 時間に基づいて法線方向に拡散
	float time = gTimeCount.time; // 時間を使用
	float3 offset = normal * time; // 基本のオフセット（法線方向）

    // 各頂点を個別にランダムに移動
	for (int i = 0; i < 3; ++i) {
		VertexShaderOutput output = input[i];

        // ランダムなオフセットを追加してバラバラに動かす
		float randomFactor = (generator.Generated1d() - 0.5) * 2.0; // -1.0から1.0の範囲のランダム値
		float3 randomOffset = normal * 0.5; // ランダムな方向に小さなオフセットを追加
		output.position.xyz += offset; // 基本のオフセットとランダムオフセットを加算

        // WVP変換
		output.position = mul(output.position, WVP);

        // 法線の計算（ワールド空間で法線を再計算）
		output.normal = normalize(mul(output.normal, (float3x3) gWorldTransformMatrix.worldInverseTranspose));
        
        // 世界空間での位置を計算
		output.worldPos = mul(output.position, gWorldTransformMatrix.world);
        
        // 頂点をストリームに追加
		triStream.Append(output);
	}
}