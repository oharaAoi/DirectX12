#include "HitSparkEffect.h"
#include "Engine/Math/MyRandom.h"

void HitSparkEffect::Init() {
	BaseCpuParticle::Init(100, "spark.obj");
}

void HitSparkEffect::Update() {
	BaseCpuParticle::Update();
}

void HitSparkEffect::MakeParticle(const Vector3& pos, uint32_t createNum) {
	uint32_t createdNum = createNum;
	for (uint32_t oi = 0; oi < kInstanceNum_; ++oi) {
		if (particleArray_[oi].lifeTime <= 0.0f) {
			particleArray_[oi].lifeTime = 0.4f;

			particleArray_[oi].acceleration = Vector3{ RandomFloat(-1, 1), RandomFloat(-1, 1) , RandomFloat(-1, 1) }.Normalize() * speed_;
			particleArray_[oi].velocity = Vector3();

		
			particleArray_[oi].scale = { .6f,.6f,.6f };
			particleArray_[oi].rotate = Quaternion::LookAt(Vector3(), particleArray_[oi].acceleration);
			particleArray_[oi].translate = pos;

			particleArray_[oi].color = { 1,1,1,1 };

			--createdNum;

			if (createdNum == 0) {
				break;
			}
		}
	}
}
