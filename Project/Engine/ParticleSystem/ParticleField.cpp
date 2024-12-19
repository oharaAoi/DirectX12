#include "ParticleField.h"
#include "Engine/Collider/CollisionFunctions.h"

ParticleField::ParticleField() {
}

ParticleField::~ParticleField() {
}

void ParticleField::Init() {
	accelerationField_.acceleration = { 6.0f, 0.0f, 0.0f };
	accelerationField_.area.min = { -1.0f, -1.0f, -1.0f };
	accelerationField_.area.max = { 1.0f, 1.0f, 1.0f };

	onField_ = false;
}

void ParticleField::Update() {
	if (!onField_) {
		return;
	}
	std::list<BaseEffect::ParticleData>& particleList = particle_->GetParticleList();
	for (std::list<BaseEffect::ParticleData>::iterator particleIter = particleList.begin(); particleIter != particleList.end();) {
		/*if (CheckCollision(accelerationField_.area, (*particleIter).transform.translate)) {
			(*particleIter).velocity += accelerationField_.acceleration * kDeltaTime_;
		}*/
		++particleIter;
	}
}

void ParticleField::Draw(const Matrix4x4& vpMatrix) const {
	if (onField_) {
		DrawAABB(accelerationField_.area, vpMatrix, { 1,0,0,1 });
	}
}

#ifdef _DEBUG
void ParticleField::Debug_Gui() {
	ImGui::Begin("ParticleFiled");
	ImGui::Checkbox("onField", &onField_);
	if (ImGui::BeginMenu("accelerationField")) {
		ImGui::DragFloat3("acceleration", &accelerationField_.acceleration.x, 0.01f);
		ImGui::DragFloat3("area.min", &accelerationField_.area.min.x, 0.01f);
		ImGui::DragFloat3("area.max", &accelerationField_.area.max.x, 0.01f);
		ImGui::EndMenu();
	}
	ImGui::End();
}
#endif

