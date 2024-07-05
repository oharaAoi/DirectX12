#pragma once
#include "Engine.h"
#include "MyRandom.h"
#include "BaseParticle.h"

const float kDeltaTime = 1.0f / 60.0f;

class Particle {
public:

	struct ParticleData {
		kTransform transform;
		Vector3 velocity;
		Vector4 color;
		float lifeTime;
		float currentTime;
	};
	
public:

	void Init(const std::string& fileName, const uint32_t& particleNum);

	void Update(const Matrix4x4& viewMat, const Matrix4x4& projection);

	void Draw();

	ParticleData MakeParticle();

private:

	// インスタンス数
	uint32_t kNumInstance_;
	uint32_t liveNumInstance_;
	// パーティクルの実体
	std::unique_ptr<BaseParticle> particles_;
	// パーティクルが持つパラメータ
	std::vector<ParticleData> particlesData_;
};

