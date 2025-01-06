#include "Particle.hlsli"

struct PerFrame {
	float time;
	float deletaTime;
};

static const int kMaxParticles = 1024;
RWStructuredBuffer<Particle> gParticles : register(u0);
RWStructuredBuffer<int> gFreeListIndex : register(u1);
ConstantBuffer<PerFrame> gPerFrame : register(b0);

[numthreads(1024, 1, 1)]
void CSmain(uint3 DTid : SV_DispatchThreadID) {
	int particleIndex = DTid.x;
	if (particleIndex < kMaxParticles) {
		if (gParticles[particleIndex].translate.y <= 0.0f) {
			gParticles[particleIndex].velocity = float3(0.0f, 0.0f, 0.0f);

		}
		
		// alphaが0は死んでいる
		if (gParticles[particleIndex].color.a >= 0.01f) {
			gParticles[particleIndex].acceleration *= gParticles[particleIndex].damping;
			gParticles[particleIndex].acceleration.y = gParticles[particleIndex].gravity;
			gParticles[particleIndex].velocity += gParticles[particleIndex].acceleration * gPerFrame.deletaTime;
			gParticles[particleIndex].velocity *= exp(-gParticles[particleIndex].damping * gPerFrame.deletaTime);
			gParticles[particleIndex].translate += gParticles[particleIndex].velocity * gPerFrame.deletaTime;
			gParticles[particleIndex].currentTime += gPerFrame.deletaTime;
			float alpha = 1.0f - (gParticles[particleIndex].currentTime / gParticles[particleIndex].lifeTime);
			gParticles[particleIndex].color.a = saturate(alpha);
		}
		else {
			// スケールに0を入れて出力されないようにする
			gParticles[particleIndex].scale = float3(0.0f, 0.0f, 0.0f);
			int freeListIndex;
			InterlockedAdd(gFreeListIndex[0], 1, freeListIndex);
			// 最新のfreeListIndexの場所に死んだparticleのIndexを設定する
			if ((freeListIndex + 1) < kMaxParticles) {
				gFreeListIndex[freeListIndex + 1] = particleIndex;
			}
			else {
				// 本来ここにはこない
				InterlockedAdd(gFreeListIndex[0], -1, freeListIndex);
			}
		}
	}
}