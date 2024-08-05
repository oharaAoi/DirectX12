#include "ParticleField.h"

ParticleField::ParticleField() {
}

ParticleField::~ParticleField() {
}

void ParticleField::Init() {
	accelerationField_.acceleration = { 15.0f, 0.0f, 0.0f };
	accelerationField_.area.min = { -1.0f, -1.0f, -1.0f };
	accelerationField_.area.max = { 1.0f, 1.0f, 1.0f };

	onField_ = false;
}

void ParticleField::Update() {

	ImGui::Begin("Field");
	ImGui::Checkbox("onField", &onField_);
	ImGui::End();

	if (!onField_) {
		return;
	}
	std::list<BaseEffect::ParticleData>& particleList = particle_->GetParticleList();
	for (std::list<BaseEffect::ParticleData>::iterator particleIter = particleList.begin(); particleIter != particleList.end();) {
		if (IsCollision(accelerationField_.area, (*particleIter).transform.translate)) {
			(*particleIter).velocity += accelerationField_.acceleration * kDeltaTime_;
		}
		++particleIter;
	}
}

void ParticleField::Draw(const Matrix4x4& vpMatrix) {
	DrawAABB(accelerationField_.area, vpMatrix, {1,0,0,1});
}

