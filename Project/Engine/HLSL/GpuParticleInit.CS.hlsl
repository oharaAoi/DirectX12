
struct Particle {
	float3 scale;
	float3 translate;
	float3 velocity;
	float lifeTime;
	float currentTime;
	float4 color;
};

static const int kMaxParticles = 1024;
RWStructuredBuffer<Particle> gParticles : register(u0);

[numthreads(1024, 1, 1)]
void CSmain(uint3 DTid : SV_DispatchThreadID) {
	int particleIndex = DTid.x;
	if (particleIndex < kMaxParticles) {
		// particle構造体の全要素を0で埋める
		gParticles[particleIndex] = (Particle) 0;
		gParticles[particleIndex].scale = float3(1.0f, 1.0f, 1.0f);
		gParticles[particleIndex].color = float4(1.0f, 1.0f, 1.0f, 1.0f);

	}
}