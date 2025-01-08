#include "Particle.hlsli"

static const int kMaxParticles = 1024;
RWStructuredBuffer<Particle> gParticles : register(u0);
RWStructuredBuffer<int> gAllAlphaZero : register(u1);

[numthreads(1, 1, 1)]
void CSmain(uint3 DTid : SV_DispatchThreadID) {
	 // 初期化
	gAllAlphaZero[0] = 1;

    // 全パーティクルを順にチェック
	for (int i = 0; i < kMaxParticles; i++) {
		if (gParticles[i].color.a > 0.0f) {
			gAllAlphaZero[0] = 0;
			break; // αが 0 でないものを見つけたら即終了
		}
	}
}