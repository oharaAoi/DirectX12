
#include "Random.hlsli"

struct Particle {
	float3 scale;
	float3 translate;
	float3 velocity;
	float lifeTime;
	float currentTime;
	float4 color;
};

struct CommonEmitter {
	float4 rotate;			// 回転(Quaternion)
	float3 translate;		// 位置
	int shape;				// emitterの形
	int count;				// 射出数
	float frequency;		// 射出間隔
	float frequencyTime;	// 時間調整用
	int emit;				// 射出許可
	float4 color;			// 色
	float speed; // 速度
};

struct SphereEmitter {
	float radius;			// 射出半径
};

struct ConeEmitter {
	float radius;			// 射出半径
	float angle;			// 角度
	float height;			// 高さ
};

struct BoxEmitter {
	float3 size;
};

struct PerFrame {
	float time;
	float deletaTime;
};


static const int kMaxParticles = 1024;
RWStructuredBuffer<Particle> gParticles : register(u0);
RWStructuredBuffer<int> gFreeListIndex : register(u1);
ConstantBuffer<PerFrame> gPerFrame : register(b0);
ConstantBuffer<CommonEmitter> gCommonEmitter : register(b1);
ConstantBuffer<SphereEmitter> gSphereEmitter : register(b2);
ConstantBuffer<ConeEmitter> gConeEmitter : register(b3);
ConstantBuffer<BoxEmitter> gBoxEmitter : register(b4);

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
	if (gCommonEmitter.emit == 1) {
		RandomGenerator generator;
		generator.seed = (DTid + gPerFrame.time) * gPerFrame.time;
		for (int countIndex = 0; countIndex < gCommonEmitter.count; ++countIndex) {
			int freeListIndex;
			// gFreeCounter[0]に1を足し、足す前の値をparticleIndexに格納
			InterlockedAdd(gFreeListIndex[0], -1, freeListIndex);
			
			if (0 <= freeListIndex && freeListIndex < kMaxParticles) {
				int particleIndex = gFreeListIndex[freeListIndex];
				gParticles[particleIndex] = (Particle) 0;
				//gParticles[particleIndex].scale = generator.Generated3d();
				gParticles[particleIndex].scale = float3(1, 1, 1);
				gParticles[particleIndex].translate = gCommonEmitter.translate + generator.Generated3d();
				gParticles[particleIndex].color.rgb = gCommonEmitter.color.rgb;
				gParticles[particleIndex].color.a = 1.0f;
				gParticles[particleIndex].lifeTime = 5.0f;
				gParticles[particleIndex].currentTime = 0.0f;
				
				if (gCommonEmitter.shape == 0) {	// sphere
					float3 randomPos = generator.Generated3d();
					randomPos.x = clamp(randomPos.x, -gSphereEmitter.radius, gSphereEmitter.radius);
					randomPos.y = clamp(randomPos.y, -gSphereEmitter.radius, gSphereEmitter.radius);
					randomPos.z = clamp(randomPos.z, -gSphereEmitter.radius, gSphereEmitter.radius);
					
					gParticles[particleIndex].translate = gCommonEmitter.translate + randomPos;
					gParticles[particleIndex].velocity = generator.Generated3d() * gCommonEmitter.speed;
				}
				else if (gCommonEmitter.shape == 1) {	// Cone
					gParticles[particleIndex].velocity = ApplyVelocityWithRotation(gCommonEmitter.rotate, float3(0, 1, 0), 0.01f) * gCommonEmitter.speed;
				}
				else if (gCommonEmitter.shape == 2) {	// box
					gParticles[particleIndex].velocity = ApplyVelocityWithRotation(gCommonEmitter.rotate, float3(0, 1, 0), 0.01f) * gCommonEmitter.speed;
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