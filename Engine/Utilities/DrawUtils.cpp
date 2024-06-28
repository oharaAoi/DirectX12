#include "DrawUtils.h"

void DrawGrid(const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix) {
	const float kGridHalfwidth_ = 2.0f;
	const uint32_t kSubdivision_ = 10;
	const float kGridEvery_ = (kGridHalfwidth_ * 4.0f) / float(kSubdivision_);

	for (uint32_t xIndex = 0; xIndex <= kSubdivision_; ++xIndex) {
		float half = float(kSubdivision_) / kGridHalfwidth_;

		Vector3 stPos = { -2 , 0, float((xIndex)-half) * kGridEvery_ / (float(kGridHalfwidth_)) };
		Vector3 endPos = { 2 , 0, float((xIndex)-half) * kGridEvery_ / (float(kGridHalfwidth_)) };
		
		if (xIndex == 5) {
			Engine::DrawLine(stPos, endPos, { 0.0f, 0.0f, 0.0f, 1.0f }, Multiply(viewMatrix, projectionMatrix));
		} else {
			Engine::DrawLine(stPos, endPos, { 1.0f, 1.0f, 1.0f, 1.0f }, Multiply(viewMatrix, projectionMatrix));
		}
	}

	for (uint32_t zIndex = 0; zIndex <= kSubdivision_; ++zIndex) {
		float half = float(kSubdivision_) / kGridHalfwidth_;

		Vector3 stPos = { float((zIndex)-half) * kGridEvery_ / (float(kGridHalfwidth_)) , 0,2 };
		Vector3 endPos = { float((zIndex)-half) * kGridEvery_ / (float(kGridHalfwidth_)) , 0, -2 };

		if (zIndex == 5) {
			Engine::DrawLine(stPos, endPos, { 0.0f, 0.0f, 0.0f, 1.0f }, Multiply(viewMatrix, projectionMatrix));
		} else {
			Engine::DrawLine(stPos, endPos, { 1.0f, 1.0f, 1.0f, 1.0f }, Multiply(viewMatrix, projectionMatrix));
		}
	}
}
