
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
	}
}