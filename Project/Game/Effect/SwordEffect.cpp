#include "SwordEffect.h"
#include "Engine/Math/MyRandom.h"
#include "Engine/Math/Easing.h"
#include "Engine/Lib/GameTimer.h"
#include "Engine/Render.h"

void SwordEffect::Init() {
	BaseCpuParticle::Init(100, "spark.obj");
}

void SwordEffect::Update() {
	perView_->viewProjection = Render::GetViewProjectionMat();
	perView_->billboardMat = Matrix4x4::MakeUnit();

	for (uint32_t oi = 0; oi < kInstanceNum_; ++oi) {
		if (particleArray_[oi].lifeTime >= 0.0f) {
			particleArray_[oi].lifeTime -= GameTimer::DeltaTime();

			float t = particleArray_[oi].lifeTime / lifeTime_;

			Vector3 scale = Vector3::Lerp(Vector3(), particleArray_[oi].scale, Ease::In::Quart(t));

			particleData_[oi].worldMat = Matrix4x4::MakeAffine(
				scale,
				particleArray_[oi].rotate,
				particleArray_[oi].translate);

			particleArray_[oi].color.w -= GameTimer::DeltaTime();

			particleData_[oi].color = particleArray_[oi].color;

		} else {
			particleData_[oi].worldMat = Matrix4x4::MakeUnit();
			particleData_[oi].color = Vector4(0, 0, 0, 0);
		}
	}
}

void SwordEffect::MakeParticle(const Vector3& pos, uint32_t createNum) {
	uint32_t createdNum = createNum;
	for (uint32_t oi = 0; oi < kInstanceNum_; ++oi) {
		if (particleArray_[oi].lifeTime <= 0.0f) {
			particleArray_[oi].lifeTime = lifeTime_;

			particleArray_[oi].acceleration = Vector3();
			particleArray_[oi].velocity = Vector3();

			Vector3 rotate = Vector3{ RandomFloat(-1, 1), RandomFloat(-1, 1) , RandomFloat(-1, 1) }.Normalize();
			particleArray_[oi].scale = { Vector3{ RandomFloat(.5f, 3), RandomFloat(.5f, 3) , RandomFloat(.5f, 3) } };
			particleArray_[oi].rotate = Quaternion::LookAt(Vector3(), rotate);
			particleArray_[oi].translate = pos;

			particleArray_[oi].color = { 1,0,0,1 };

			--createdNum;

			if (createdNum == 0) {
				break;
			}
		}
	}
}
