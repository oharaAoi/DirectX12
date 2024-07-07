#include "Metallic.h"

Metallic::Metallic() {
	Init();
}

Metallic::~Metallic() {
}

void Metallic::Init() {
	camera_ = std::make_unique<Camera>();

	metallicParameter_ = 0.0f;
	roughnessParameter_ = 0.5f;

	for (int32_t oi = 0; oi < 10; oi++) {
		spheres_.push_back(Engine::CreateSphere(16));
		Vector3 scale = { 0.5f, 0.5f, 0.5f };

		Vector3 translate = { oi - 5.0f, 0.0f, 0.0f };
		transforms_.push_back({ scale, {0.0f, 0.0f, 0.0f}, translate });

		roughness_.push_back(0.5f);
		metallic_.push_back(0.1f * (float)oi);
	}
}

void Metallic::Update() {
	camera_->Update();

	for (int32_t oi = 0; oi < spheres_.size(); oi++) {
		Matrix4x4 worldMat = MakeAffineMatrix(transforms_[oi]);
		spheres_[oi]->Update(worldMat, camera_->GetViewMatrix(), camera_->GetProjectionMatrix());

		spheres_[oi]->SetMaterials(roughness_[oi], metallic_[oi]);
	}

	ImGui::Begin("Set");
	if (ImGui::CollapsingHeader("Metallic")) {
		ImGui::DragFloat("metallic", &metallicParameter_, 0.01f, 0.0f, 1.0f);
		for (int32_t oi = 0; oi < 10; oi++) {
			metallic_[oi] = metallicParameter_;
			roughness_[oi] = 0.1f * (float)oi;
		}
	}
	if (ImGui::CollapsingHeader("Rouhness")) {
		ImGui::DragFloat("roughness", &roughnessParameter_, 0.01f, 0.0f, 1.0f);
		for (int32_t oi = 0; oi < 10; oi++) {
			roughness_[oi] = roughnessParameter_;
			metallic_[oi] = 0.1f * (float)oi;
		}
	}
	ImGui::End();

	Engine::SetEyePos(camera_->GetWorldTranslate());
}

void Metallic::Draw() {
	for (int32_t oi = 0; oi < spheres_.size(); oi++) {
		Engine::DrawSphere(spheres_[oi].get());
	}
}
