#include "EnemyDownEffect.h"
#include "Engine/Math/MyRandom.h"
#include "Engine/Math/Easing.h"
#include "Engine/Lib/GameTimer.h"
#include "Engine/Render.h"

void EnemyDownEffect::Init() {
	BaseCpuParticle::Init(100, "cube.obj");
}

void EnemyDownEffect::Update() {
	perView_->viewProjection = Render::GetViewProjectionMat();
	perView_->billboardMat = Matrix4x4::MakeUnit();

	for (uint32_t oi = 0; oi < kInstanceNum_; ++oi) {
		if (particleArray_[oi].lifeTime >= 0.0f) {
			particleArray_[oi].lifeTime -= GameTimer::DeltaTime();

			float t = particleArray_[oi].lifeTime / 2.0f;

			particleArray_[oi].acceleration.y += particleArray_[oi].gravity * GameTimer::DeltaTime();

			Vector3 accele{};
			accele.y = particleArray_[oi].acceleration.y;
			
			Vector3 velo{};
			velo.x = particleArray_[oi].velocity.x * .8f;
			velo.z = particleArray_[oi].velocity.z * .8f;
			particleArray_[oi].velocity.x = velo.x;
			particleArray_[oi].velocity.z = velo.z;

			particleArray_[oi].velocity += accele * GameTimer::DeltaTime();
			particleArray_[oi].translate += particleArray_[oi].velocity;

			if (particleArray_[oi].translate.y < 0.0f) {
				particleArray_[oi].translate.y = 0.0f;
			}

			Vector3 scale = Vector3::Lerp(Vector3(), particleArray_[oi].scale, Ease::Out::Quart(t));

			particleData_[oi].worldMat = Matrix4x4::MakeAffine(
				scale,
				particleArray_[oi].rotate,
				particleArray_[oi].translate);

			particleArray_[oi].color.w = std::lerp(0.0f, 1.0f, t);

			particleData_[oi].color = particleArray_[oi].color;

		} else {
			particleData_[oi].worldMat = Matrix4x4::MakeUnit();
			particleData_[oi].color = Vector4(0, 0, 0, 0);
		}
	}
}

void EnemyDownEffect::MakeParticle(const Vector3& pos, uint32_t createNum) {
	uint32_t createdNum = createNum;
	for (uint32_t oi = 0; oi < kInstanceNum_; ++oi) {
		if (particleArray_[oi].lifeTime <= 0.0f) {
			particleArray_[oi].lifeTime = 2.0f;

			particleArray_[oi].acceleration = Vector3{ 0.0f, 10.0f , 0.0f }.Normalize();
			particleArray_[oi].velocity = Vector3{ RandomFloat(-1, 1), 0.0f , RandomFloat(-1, 1) }.Normalize();


			particleArray_[oi].scale = Vector3{ RandomFloat(.2f, .5f), RandomFloat(.2f, .5f) , RandomFloat(.2f, .5f) };
			particleArray_[oi].rotate = Quaternion(0,0,0,1);
			particleArray_[oi].translate = pos;

			particleArray_[oi].gravity = -9.8f;

			particleArray_[oi].color = Vector4{ RandomFloat(0, .8f), RandomFloat(0, .8f) , RandomFloat(0, .8f), 1.0f };

			--createdNum;

			if (createdNum == 0) {
				break;
			}
		}
	}
}
