
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
RWStructuredBuffer<int> gFreeCounter : register(u1);

[numthreads(1024, 1, 1)]
void CSmain(uint3 DTid : SV_DispatchThreadID) {
	int countIndex = DTid.x;
	
	if (countIndex == 0) {
		gFreeCounter[0] = 0;
	}
	
	gParticles[countIndex] = (Particle) 0;
	gParticles[countIndex].scale = float3(0.0f, 0.0f, 0.0f);
	gParticles[countIndex].color.rgba = float4(0.0f, 0.0f, 0.0f, 1.0f);
}