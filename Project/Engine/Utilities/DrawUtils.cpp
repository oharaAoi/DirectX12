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
			Render::DrawLine(stPos, endPos, { 0.0f, 0.0f, 0.0f, 1.0f }, Multiply(viewMatrix, projectionMatrix));
		} else {
			Render::DrawLine(stPos, endPos, { 1.0f, 1.0f, 1.0f, 1.0f }, Multiply(viewMatrix, projectionMatrix));
		}
	}

	for (uint32_t zIndex = 0; zIndex <= kSubdivision_; ++zIndex) {
		float half = float(kSubdivision_) / kGridHalfwidth_;

		Vector3 stPos = { float((zIndex)-half) * kGridEvery_ / (float(kGridHalfwidth_)) , 0,2 };
		Vector3 endPos = { float((zIndex)-half) * kGridEvery_ / (float(kGridHalfwidth_)) , 0, -2 };

		if (zIndex == 5) {
			Render::DrawLine(stPos, endPos, { 0.0f, 0.0f, 0.0f, 1.0f }, Multiply(viewMatrix, projectionMatrix));
		} else {
			Render::DrawLine(stPos, endPos, { 1.0f, 1.0f, 1.0f, 1.0f }, Multiply(viewMatrix, projectionMatrix));
		}
	}
}

void DrawAABB(const AABB& aabb, const Matrix4x4& vpMatrix, const Vector4& color) {
	std::array<Vector3, 8> point = {
		Vector3{aabb.min.x,aabb.max.y, aabb.min.z }, // front_LT
		Vector3{aabb.max.x,aabb.max.y, aabb.min.z }, // front_RT
		Vector3{aabb.max.x,aabb.min.y, aabb.min.z }, // front_RB
		Vector3{aabb.min.x,aabb.min.y, aabb.min.z }, // front_LB
		Vector3{aabb.min.x,aabb.max.y, aabb.max.z }, // back_LT
		Vector3{aabb.max.x,aabb.max.y, aabb.max.z }, // back_RT
		Vector3{aabb.max.x,aabb.min.y, aabb.max.z }, // back_RB
		Vector3{aabb.min.x,aabb.min.y, aabb.max.z }, // back_LB
	};

	for (uint32_t oi = 0; oi < 4; oi++) {
		Render::DrawLine(point[oi], point[(oi + 1) % 4], color, vpMatrix);
		uint32_t j = oi + 4;
		Render::DrawLine(point[j], point[(j + 1) % 4 + 4], color, vpMatrix);
		Render::DrawLine(point[oi], point[j], color, vpMatrix);
	}
}

void DrawOBB(const OBB& obb, const Matrix4x4& vpMatrix, const Vector4& color) {
	Matrix4x4 rotateMatrix = obb.matRotate;
	// 平行移動分を作成
	Matrix4x4 matTranslate = MakeTranslateMatrix(obb.center);
	// 拡縮分
	Matrix4x4 matScale = MakeScaleMatrix({ 1.0f,1.0f, 1.0f });
	// ワールド行列を求める
	Matrix4x4 worldMat = matScale * rotateMatrix * matTranslate;

	// ローカルの頂点を求める
	Vector3 min = obb.size * -1;
	Vector3 max = obb.size;
	std::array<Vector3, 8> point = {
		Vector3{min.x, max.y, min.z } * worldMat, // front_LT
		Vector3{max.x, max.y, min.z } * worldMat, // front_RT
		Vector3{max.x, min.y, min.z } * worldMat, // front_RB
		Vector3{min.x, min.y, min.z } * worldMat, // front_LB
		Vector3{min.x, max.y, max.z } * worldMat, // back_LT
		Vector3{max.x, max.y, max.z } * worldMat, // back_RT
		Vector3{max.x, min.y, max.z } * worldMat, // back_RB
		Vector3{min.x, min.y, max.z } * worldMat, // back_LB
	};


	for (uint32_t oi = 0; oi < 4; oi++) {
		Render::DrawLine(point[oi], point[(oi + 1) % 4], color, vpMatrix);
		uint32_t j = oi + 4;
		Render::DrawLine(point[j], point[(j + 1) % 4 + 4], color, vpMatrix);
		Render::DrawLine(point[oi], point[j], color, vpMatrix);
	}
}
