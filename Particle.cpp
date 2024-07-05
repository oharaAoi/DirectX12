#include "Particle.h"

void Particle::Init(const std::string& fileName, const uint32_t& particleNum) {
	kNumInstance_ = particleNum;
	particles_ = Engine::CreateBaseParticle(fileName, particleNum);
	particlesData_.resize(kNumInstance_);

	for (uint32_t oi = 0; oi < kNumInstance_; oi++) {
		particlesData_[oi] = MakeParticle();
	}
}

void Particle::Update(const Matrix4x4& viewMat, const Matrix4x4& projection) {
	liveNumInstance_ = 0;
	for (uint32_t oi = 0; oi < kNumInstance_; oi++) {
		if (particlesData_[oi].lifeTime <= particlesData_[oi].currentTime) {
			continue;
		}
		particlesData_[oi].currentTime += kDeltaTime;
		particlesData_[oi].transform.translate += particlesData_[oi].velocity * kDeltaTime;
		Matrix4x4 worldMat = MakeAffineMatrix(particlesData_[oi].transform);
		particles_->Update(worldMat, viewMat, projection, particlesData_[oi].color, oi);
		particlesData_[oi].color.w = 1.0f - (particlesData_[oi].currentTime / particlesData_[oi].lifeTime);
		liveNumInstance_++;
	}
}


void Particle::Draw() {
	Engine::DrawParticle(particles_.get(), liveNumInstance_);
}

Particle::ParticleData Particle::MakeParticle() {
	ParticleData data{};
	data.transform.scale;

	data.transform.scale = { 1,1,1 };
	data.transform.rotate = { 0,0,0 };
	data.transform.translate = { RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f) };
	// velocityの設定
	data.velocity = { RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f) };
	// colorの設定
	data.color = { RandomFloat(0.0f, 1.0f), RandomFloat(0.0f, 1.0f), RandomFloat(0.0f, 1.0f) ,RandomFloat(0.0f, 1.0f) };
	// lifeTimeの設定
	data.lifeTime = RandomFloat(1.0f, 3.0f);
	data.currentTime = 0;

	return data;
}
