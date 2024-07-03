#include "Particle.h"

void Particle::Init(const std::string& fileName) {
	particles_ = Engine::CreateBaseParticle(fileName);
	
	transform_ = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
}

void Particle::Update(const Matrix4x4& viewMat, const Matrix4x4& projection) {
	for (uint32_t oi = 0; oi < 10; oi++) {
		kTransform transform;
		transform.scale = { 1,1,1 };
		transform.rotate = { 0,0,0 };
		transform.translate = { oi * 0.1f, oi * 0.1f, oi * 0.1f };
		Matrix4x4 worldMat = MakeAffineMatrix(transform);
		particles_->Update(worldMat, viewMat, projection, oi);
	}
}

void Particle::Draw() {
	Engine::DrawParticle(particles_.get());
}
