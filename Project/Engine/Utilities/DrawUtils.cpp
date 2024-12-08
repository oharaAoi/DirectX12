#include "DrawUtils.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Grid線の描画
//////////////////////////////////////////////////////////////////////////////////////////////////

void DrawGrid(const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix) {
	//const float kGridHalfwidth_ = 20.0f; // 中心からの半幅
	const uint32_t kSubdivision_ = 60;   // 分割数
	const float gridLength = 30.0f;      // グリッドの全体幅
	const float kGridEvery_ = 1.0f;      // グリッドの1マスのサイズ

	// X軸に平行なグリッド線の描画
	for (uint32_t xIndex = 0; xIndex <= kSubdivision_; ++xIndex) {
		float half = float(kSubdivision_) / 2.0f;

		// 始点と終点
		Vector3 stPos = { -gridLength, 0, (xIndex - half) * kGridEvery_ };
		Vector3 endPos = { gridLength, 0, (xIndex - half) * kGridEvery_ };

		// 中央軸ラインの色を変更
		if (xIndex == kSubdivision_ / 2) {
			Render::DrawLine(stPos, endPos, { 0.0f, 0.0f, 1.0f, 1.0f }, Multiply(viewMatrix, projectionMatrix));
		} else {
			// 他のグリッド線
			Render::DrawLine(stPos, endPos, { 0.8f, 0.8f, 0.8f, 1.0f }, Multiply(viewMatrix, projectionMatrix));
		}
	}

	// Z軸に平行なグリッド線の描画
	for (uint32_t zIndex = 0; zIndex <= kSubdivision_; ++zIndex) {
		float half = float(kSubdivision_) / 2.0f;

		// 始点と終点
		Vector3 stPos = { (zIndex - half) * kGridEvery_, 0, gridLength };
		Vector3 endPos = { (zIndex - half) * kGridEvery_, 0, -gridLength };

		// 中央軸ラインの色を変更
		if (zIndex == kSubdivision_ / 2) {
			Render::DrawLine(stPos, endPos, { 1.0f, 0.0f, 0.0f, 1.0f }, Multiply(viewMatrix, projectionMatrix));
		} else {
			// 他のグリッド線
			Render::DrawLine(stPos, endPos, { 0.8f, 0.8f, 0.8f, 1.0f }, Multiply(viewMatrix, projectionMatrix));
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　球の描画
//////////////////////////////////////////////////////////////////////////////////////////////////

void DrawSphere(const Vector3& center, float radius, const Matrix4x4& viewProjectionMatrix) {
	const uint32_t kSubdivision = 16;
	const float kLonEvery = 2.0f * float(M_PI) / kSubdivision;
	const float kLatEvery = float(M_PI) / kSubdivision;

	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -float(M_PI) / 2.0f + kLatEvery * latIndex; // 現在の緯度 theta

		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery; // 現在の経度 fai

			Vector3 a, b, c;
			Vector3 localA{}, localB{}, localC{};
			localA = {
				std::cos(lat) * cos(lon) * (radius),
				std::sin(lat) * (radius),
				std::cos(lat) * std::sin(lon) * (radius)
			};

			localB = {
				std::cos(lat + kLatEvery) * std::cos(lon) * (radius) ,
				std::sin(lat + kLatEvery) * (radius),
				std::cos(lat + kLatEvery) * std::sin(lon) * (radius)
			};

			localC = {
				std::cos(lat) * std::cos(lon + kLonEvery) * (radius),
				std::sin(lat) * (radius),
				std::cos(lat) * std::sin(lon + kLonEvery) * (radius)
			};


			a = {
				localA.x + center.x,
				localA.y + center.y,
				localA.z + center.z,
			};

			b = {
				localB.x + center.x,
				localB.y + center.y,
				localB.z + center.z,
			};

			c = {
				localC.x + center.x,
				localC.y + center.y,
				localC.z + center.z,
			};

			Render::DrawLine(a, b, { 1.0f, 0.0f, 0.0f, 1.0f }, viewProjectionMatrix);
			Render::DrawLine(a, c, { 1.0f, 0.0f, 0.0f, 1.0f }, viewProjectionMatrix);
		}
	}
}

void DrawCone(const Vector3& center, const Quaternion& rotate, float radius, float angle, float height, const Matrix4x4& viewProjectionMatrix) {
	const uint32_t segment = 36;
	float angleIncrement = 2.0f * PI / segment;
	const Vector3 rotateHeight = rotate * Vector3(0, height, 0);
	const Vector3 coneTip = center + rotateHeight; // 円錐の頂点（Y軸方向を高さとして仮定）
	float newRadius = height * std::tanf(angle * 0.5f);

	std::vector<Vector3> basePoints(segment);
	for (int i = 0; i < segment; ++i) {
		float theta = i * angleIncrement;
		basePoints[i] = center + radius * Vector3(cos(theta), 0, sin(theta));
		basePoints[i] = (rotate * (basePoints[i] - center)) + center;  // 回転適用
	}

	std::vector<Vector3> topPoints(segment);
	for (int i = 0; i < segment; ++i) {
		float theta = i * angleIncrement;
		topPoints[i] = center + newRadius * Vector3(cos(theta), 0, sin(theta));
		topPoints[i] = (rotate * (topPoints[i] - center)) + center;  // 回転適用
	}

	// 描画
	for (int i = 0; i < segment; ++i) {
		// 現在の点と次の点を結ぶ（円の線）
		Vector3 p1 = basePoints[i];
		Vector3 p2 = basePoints[(i + 1) % segment];

		Vector3 topP1 = topPoints[i] + rotateHeight;
		Vector3 topP2 = topPoints[(i + 1) % segment] + rotateHeight;

		Render::DrawLine(p1, p2, { 0.0f, 1.0f, 0.0f, 1.0f }, viewProjectionMatrix);
		Render::DrawLine(topP1, topP2, { 0.0f, 1.0f, 0.0f, 1.0f }, viewProjectionMatrix);

		if (i % 9 == 0) {
			// 頂点と底面の点を結ぶ（側面の線）
			Render::DrawLine(topP1, p1, { 0.0f, 1.0f, 0.0f, 1.0f }, viewProjectionMatrix);
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　AABBの描画
//////////////////////////////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　OBBの描画
//////////////////////////////////////////////////////////////////////////////////////////////////

void DrawOBB(const OBB& obb, const Matrix4x4& vpMatrix, const Vector4& color) {
	Matrix4x4 rotateMatrix = obb.matRotate;
	// 平行移動分を作成
	Matrix4x4 matTranslate = obb.center.MakeTranslateMat();
	// 拡縮分
	Matrix4x4 matScale = Vector3({ 1.0f,1.0f, 1.0f }).MakeScaleMat();
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
