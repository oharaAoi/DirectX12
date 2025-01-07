#include "Random.hlsli"
#include "Particle.hlsli"

struct CommonEmitter {
	float4 rotate; // 回転(Quaternion)
	float3 translate; // 位置
	int shape; // emitterの形
	int count; // 射出数
	float frequency; // 射出間隔
	float frequencyTime; // 時間調整用
	int emit; // 射出許可
	
	// particle自体の情報
	float4 color; // 色
	float3 minScale; // 最小の大きさ
	float3 maxScale; // 最大の大きさ
	float speed; // 速度
	float lifeTime;
	float gravity;
	float damping;
};

struct SphereEmitter {
	float radius; // 射出半径
};

struct ConeEmitter {
	float radius; // 射出半径
	float angle; // 角度
	float height; // 高さ
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
				// ここは良くない処理
				int particleIndex = countIndex;
				gParticles[particleIndex] = (Particle) 0;
				//gParticles[particleIndex].scale = generator.Generated3d();
				float x = generator.Generated1dRange(gCommonEmitter.minScale.x, gCommonEmitter.maxScale.x);
				float y = generator.Generated1dRange(gCommonEmitter.minScale.y, gCommonEmitter.maxScale.y);
				float z = generator.Generated1dRange(gCommonEmitter.minScale.z, gCommonEmitter.maxScale.z);
				
				gParticles[particleIndex].scale = float3(x, y, z);
				gParticles[particleIndex].rotate = float3(0, 0, 0);
				gParticles[particleIndex].translate = gCommonEmitter.translate + generator.Generated3d();
				gParticles[particleIndex].color.rgb = gCommonEmitter.color.rgb;
				gParticles[particleIndex].color.a = gCommonEmitter.color.a;
				gParticles[particleIndex].lifeTime = gCommonEmitter.lifeTime;
				gParticles[particleIndex].currentTime = 0.0f;
				gParticles[particleIndex].acceleration = float3(0, 0, 0);
				gParticles[particleIndex].damping = gCommonEmitter.damping;
				gParticles[particleIndex].gravity = gCommonEmitter.gravity;
				
				if (gCommonEmitter.shape == 0) { // sphere
					gParticles[particleIndex].translate = gCommonEmitter.translate;
					gParticles[particleIndex].velocity = generator.Generated3d() * gCommonEmitter.speed;
				}
				else if (gCommonEmitter.shape == 1) { // Cone
					float angle = generator.Generated1dRange(-gConeEmitter.angle, gConeEmitter.angle);
					gParticles[particleIndex].velocity = ApplyVelocityWithRotation(gCommonEmitter.rotate, float3(cos(angle), 1, sin(angle)), 0.01f) * gCommonEmitter.speed;
				}
				else if (gCommonEmitter.shape == 2) { // box
					float x = generator.Generated1dRange(-gBoxEmitter.size.x, gBoxEmitter.size.x);
					float y = generator.Generated1dRange(-gBoxEmitter.size.y, gBoxEmitter.size.y);
					float z = generator.Generated1dRange(-gBoxEmitter.size.z, gBoxEmitter.size.z);
					
					gParticles[particleIndex].translate = gCommonEmitter.translate;
					gParticles[particleIndex].translate.x += x;
					gParticles[particleIndex].translate.y += y;
					gParticles[particleIndex].translate.z += z;
					
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