
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
	float3 translate;		// 位置
	float radius;			// 射出半径
	int count;				// 射出数
	float frequency;		// 射出間隔
	float frequencyTime;	// 時間調整用
	int emit;				// 射出許可
};

struct ConeEmitter {
	float3 translate;		// 位置
	float radius;			// 射出半径
	float angle;			// 角度
	float height;			// 高さ
	int count;				// 射出数
	float frequency;		// 射出間隔
	float frequencyTime;	// 時間調整用
	int emit;				// 射出許可
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
				gParticles[particleIndex].scale = generator.Generated3d();
				gParticles[particleIndex].translate = gEmitter.translate + generator.Generated3d();
				gParticles[particleIndex].color.rgb = generator.Generated3d();
				gParticles[particleIndex].color.a = 1.0f;
				gParticles[particleIndex].lifeTime = 5.0f;
				gParticles[particleIndex].currentTime = 0.0f;
				gParticles[particleIndex].velocity = generator.Generated3d();
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