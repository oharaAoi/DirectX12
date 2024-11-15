
struct Particle {
	float3 scale;
	float3 translate;
	float3 velocity;
	float lifeTime;
	float currentTime;
	float4 color;
};

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
		// alphaが0は死んでいる
		if (gParticles[particleIndex].color.a != 0) {
			gParticles[particleIndex].translate += gParticles[particleIndex].velocity * gPerFrame.deletaTime;
			gParticles[particleIndex].currentTime += gPerFrame.deletaTime;
			float alpha = 1.0f - (gParticles[particleIndex].currentTime / gParticles[particleIndex].lifeTime);
			gParticles[particleIndex].color.a = saturate(alpha);
		}
		
		// alphaが0になったのでFreeにする
		if (gParticles[particleIndex].color.a == 0) {
			// スケールに0を入れて出力されないようにする
			gParticles[particleIndex].scale = float3(0.0f, 0.0f, 0.0f);
			int freeListIndex;
			InterlockedAdd(gFreeListIndex[0], 1, freeListIndex);
			// 最新のfreeListIndexの場所に死んだparticleのIndexを設定する
			if ((freeListIndex + 1) < kMaxParticles) {
				gFreeListIndex[freeListIndex + 1] = particleIndex;
			} else {
				// 本来ここにはこない
				InterlockedAdd(gFreeListIndex[0], -1, freeListIndex);
			}
			
			
		}

	}
}