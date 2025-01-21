#include "Particle.hlsli"

static const int kMaxParticles = 1024;
RWStructuredBuffer<Particle> gParticles : register(u0);
RWStructuredBuffer<int> gFreeListIndex : register(u1);
RWStructuredBuffer<uint> gFreeList : register(u2);

[numthreads(1024, 1, 1)]
void CSmain(uint3 DTid : SV_DispatchThreadID) {
	int countIndex = DTid.x;
	
	if (countIndex == 0) {
		gFreeListIndex[0] = kMaxParticles - 1;
		gFreeList[0] = 0;
	}
	
	if (countIndex < kMaxParticles) {
		gParticles[countIndex] = (Particle) 0;
		gFreeList[countIndex] = countIndex;

	}
}