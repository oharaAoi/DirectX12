
struct Particle {
	float3 scale;
	float3 translate;
	float3 velocity;
	float lifeTime;
	float currentTime;
	float4 color;
};

struct SphereEmitter {
	float3 translate; // 位置
	float radius; // 射出半径
	int count; // 射出数
	float frequency; // 射出間隔
	float frequencyTime; // 時間調整用
	int emit; // 射出許可
};

struct PerFrame {
	float time;
	float deletaTime;
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

static const int kMaxParticles = 1024;
RWStructuredBuffer<Particle> gParticles : register(u0);
RWStructuredBuffer<int> gFreeCounter : register(u1);
ConstantBuffer<SphereEmitter> gEmitter : register(b0);
ConstantBuffer<PerFrame> gPerFrame : register(b1);

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

[numthreads(1, 1, 1)]
void CSmain(uint3 DTid : SV_DispatchThreadID) {
	if (gEmitter.emit == 1) {
		RandomGenerator generator;
		generator.seed = (DTid + gPerFrame.time) * gPerFrame.time;
		for (int countIndex = 0; countIndex < gEmitter.count; ++countIndex) {
			int particleIndex;
			// gFreeCounter[0]に1を足し、足す前の値をparticleIndexに格納
			InterlockedAdd(gFreeCounter[0], 1, particleIndex);
			if (particleIndex < kMaxParticles) {
			// カウント分射出
				gParticles[particleIndex] = (Particle) 0;
				gParticles[particleIndex].scale = generator.Generated3d();
				gParticles[particleIndex].translate = generator.Generated3d();
				gParticles[particleIndex].color.rgb = generator.Generated3d();
				gParticles[particleIndex].color.a = 1.0f;
				gParticles[particleIndex].lifeTime = 5.0f;
				gParticles[particleIndex].currentTime = 0.0f;
				gParticles[particleIndex].velocity = generator.Generated3d();
			}
		}
	}
}