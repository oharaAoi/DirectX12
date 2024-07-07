#include "Comparison.h"

Comparison::Comparison() {
	Init();
}

Comparison::~Comparison() {
}

void Comparison::Init() {
	camera_ = std::make_unique<Camera>();

	for (int32_t row = 0; row < 6; row++) {
		for (int32_t col = 0; col < 6; col++) {
			spheres_.push_back(Engine::CreateSphere(16));
			Vector3 scale = { 0.4f, 0.4f, 0.4f };
			Vector3 translate = { col - 3.0f, row - 3.0f, 0.0f };
			transforms_.push_back({ scale, {0.0f, 0.0f, 0.0f}, translate });

			roughness_.push_back(0.2f * (float)col);
			metallic_.push_back(0.2f * (float)row);
		}
	}
}

void Comparison::Update() {
	camera_->Update();

	for (int32_t oi = 0; oi < spheres_.size(); oi++) {
		Matrix4x4 worldMat = MakeAffineMatrix(transforms_[oi]);
		spheres_[oi]->Update(worldMat, camera_->GetViewMatrix(), camera_->GetProjectionMatrix());

		spheres_[oi]->SetMaterials(roughness_[oi], metallic_[oi]);
	}


	Engine::SetEyePos(camera_->GetWorldTranslate());
}

void Comparison::Draw() {
	for (int32_t oi = 0; oi < spheres_.size(); oi++) {
		Engine::DrawSphere(spheres_[oi].get());
	}
}
