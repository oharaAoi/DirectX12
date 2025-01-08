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
				std::cos(lat) * cos(lon) * (radius / 2),
				std::sin(lat) * (radius / 2),
				std::cos(lat) * std::sin(lon) * (radius / 2)
			};

			localB = {
				std::cos(lat + kLatEvery) * std::cos(lon) * (radius / 2) ,
				std::sin(lat + kLatEvery) * (radius / 2),
				std::cos(lat + kLatEvery) * std::sin(lon) * (radius / 2)
			};

			localC = {
				std::cos(lat) * std::cos(lon + kLonEvery) * (radius / 2),
				std::sin(lat) * (radius / 2),
				std::cos(lat) * std::sin(lon + kLonEvery) * (radius / 2)
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

void DrawOBB(const OBB& obb, const Vector4& color) {
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
		Render::DrawLine(point[oi], point[(oi + 1) % 4], color);
		uint32_t j = oi + 4;
		Render::DrawLine(point[j], point[(j + 1) % 4 + 4], color);
		Render::DrawLine(point[oi], point[j], color);
	}
}

#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
void DrawMatrix(const Matrix4x4& mat, const std::string& id) {
	Vector4 vector1 = Vector4(mat.m[0][0], mat.m[0][1], mat.m[0][2], mat.m[0][3]);
	Vector4 vector2 = Vector4(mat.m[1][0], mat.m[1][1], mat.m[1][2], mat.m[1][3]);
	Vector4 vector3 = Vector4(mat.m[2][0], mat.m[2][1], mat.m[2][2], mat.m[2][3]);
	Vector4 vector4 = Vector4(mat.m[3][0], mat.m[3][1], mat.m[3][2], mat.m[3][3]);

	ImGui::Text(id.c_str());
	ImGui::Separator();
	ImGui::Text("%f, %f, %f, %f", vector1.x, vector1.y, vector1.z, vector1.w);
	ImGui::Spacing();
	ImGui::Text("%f, %f, %f, %f", vector2.x, vector2.y, vector2.z, vector2.w);
	ImGui::Spacing();
	ImGui::Text("%f, %f, %f, %f", vector3.x, vector3.y, vector3.z, vector3.w);
	ImGui::Spacing();
	ImGui::Text("%f, %f, %f, %f", vector4.x, vector4.y, vector4.z, vector4.w);
	ImGui::Separator();
}
void DrawQuaternion(const Quaternion& q, const std::string& id) {
	ImGui::Text(id.c_str());
	ImGui::SameLine();
	ImGui::Text(": %f, %f, %f, %f\n", q.x, q.y, q.z, q.w);
}
void DrawVector(const Vector3& vector, const std::string& id) {
	ImGui::Text(id.c_str());
	ImGui::SameLine();
	ImGui::Text(": %f, %f, %f\n", vector.x, vector.y, vector.z);
}
#endif