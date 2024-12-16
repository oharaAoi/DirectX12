#include "Random.hlsli"

struct Particle {
	float3 scale;
	float3 translate;
	float3 velocity;
	float lifeTime;
	float currentTime;
	float4 color;
};

struct SphereEmitter {
	float4 rotate;			// 回転(Quaternion)
	float3 translate;		// 位置
	int shape;				// emitterの形
	int count;				// 射出数
	float frequency;		// 射出間隔
	float frequencyTime;	// 時間調整用
	int emit;				// 射出許可
	float4 color;			// 色
	float speed;			// 速度
	float radius;			// 半径
	int emissionType;		// 放射の種類
};
struct PerFrame {
	float time;
	float deletaTime;
};

static const int kMaxParticles = 1024;
RWStructuredBuffer<Particle> gParticles : register(u0);
RWStructuredBuffer<int> gFreeListIndex : register(u1);
ConstantBuffer<SphereEmitter> gEmitter : register(b0);
ConstantBuffer<PerFrame> gPerFrame : register(b1);

float3 ApplyVelocityWithRotation(float4 rotation, float3 localVelocity, float threshold) {
    // クォータニオンの長さを計算
	float rotationMagnitude = sqrt(dot(rotation, rotation));
    
    // 閾値を超えない場合、ワールド上方向 (0, 1, 0) を使用
	if (rotationMagnitude < threshold) {
		return float3(0, 1, 0);
	}

    // クォータニオンを正規化
	rotation /= rotationMagnitude;

    // クォータニオンを使ってローカル速度を回転
	float3 u = cross(rotation.xyz, localVelocity);
	float3 v = cross(rotation.xyz, u) + u * rotation.w;

    // ワールド座標系の速度
	return localVelocity + 2.0 * v;
}

[numthreads(1, 1, 1)]
void CSmain(uint3 DTid : SV_DispatchThreadID) {
	if (gEmitter.emit == 1) {
		RandomGenerator generator;
		generator.seed = (DTid + gPerFrame.time) * gPerFrame.time;
		for (int countIndex = 0; countIndex < gEmitter.count; ++countIndex) {
			int freeListIndex;
			// gFreeCounter[0]に1を足し、足す前の値をparticleIndexに格納
			InterlockedAdd(gFreeListIndex[0], -1, freeListIndex);
			
			if (0 <= freeListIndex && freeListIndex < kMaxParticles) {
				int particleIndex = gFreeListIndex[freeListIndex];
				gParticles[particleIndex] = (Particle) 0;
				//gParticles[particleIndex].scale = generator.Generated3d();
				gParticles[particleIndex].scale = float3(1, 1, 1);
				gParticles[particleIndex].translate = gEmitter.translate + generator.Generated3d();
				gParticles[particleIndex].color.rgb = gEmitter.color.rgb;
				gParticles[particleIndex].color.a = 1.0f;
				gParticles[particleIndex].lifeTime = 5.0f;
				gParticles[particleIndex].currentTime = 0.0f;
				
				float3 randomPos = generator.Generated3d();
				randomPos.x = clamp(randomPos.x, -gEmitter.radius, gEmitter.radius);
				randomPos.y = clamp(randomPos.y, -gEmitter.radius, gEmitter.radius);
				randomPos.z = clamp(randomPos.z, -gEmitter.radius, gEmitter.radius);
					
				gParticles[particleIndex].translate = gEmitter.translate + randomPos;
				
				if (gEmitter.emissionType == 0) {
					gParticles[particleIndex].velocity = generator.Generated3d() * gEmitter.speed;
				}
				else {
					gParticles[particleIndex].velocity = ApplyVelocityWithRotation(gEmitter.rotate, float3(0, 1, 0), 0.01f) * gEmitter.speed;
				}
			}
			else {
				// 発生しなかったので減らした分を元に戻す
				InterlockedAdd(gFreeListIndex[0], 1);
				// これ以上発生しないのでループを抜ける
				break;
			}
		}
	}
}