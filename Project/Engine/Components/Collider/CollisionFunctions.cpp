#include "CollisionFunctions.h"
#include "Engine/Lib/Math/MyMath.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　球同士の当たり判定
//////////////////////////////////////////////////////////////////////////////////////////////////

bool CheckCollision(const Sphere& s1, const Sphere& s2) {
	// 中心点間の距離を求める
	float distance = Length(s1.center - s2.center);
	// 半径の合計よりも短ければ衝突
	if (distance <= s1.radius + s2.radius) {
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　OBB同士の当たり判定
//////////////////////////////////////////////////////////////////////////////////////////////////

bool CheckCollison(const OBB& colliderA, const OBB& colliderB) {
	// 分離軸の組み合わせを取得
	std::vector<Vector3> crossSeparatingAxises;
	for (uint8_t obb1Index = 0; obb1Index < 3; obb1Index++) {
		for (uint8_t obb2Index = 0; obb2Index < 3; obb2Index++) {
			Vector3 crossAxis = Cross(colliderA.orientations[obb1Index], colliderB.orientations[obb2Index]);
			if (Length(crossAxis) > kEpsilon) { // 長さが非常に小さい場合を除外
				Normalize(crossAxis);
				crossSeparatingAxises.push_back(crossAxis);
			}
		}
	}

	// 面法線をまとめる
	std::vector<Vector3> obb1SurfaceNormals;
	for (uint8_t obbIndex = 0; obbIndex < 3; obbIndex++) {
		obb1SurfaceNormals.push_back(Normalize(TransformNormal(colliderA.orientations[obbIndex], colliderA.matRotate)));
	}

	std::vector<Vector3> obb2SurfaceNormals;
	for (uint8_t obbIndex = 0; obbIndex < 3; obbIndex++) {
		obb2SurfaceNormals.push_back(Normalize(TransformNormal(colliderB.orientations[obbIndex], colliderB.matRotate)));
	}

	// ------------------------------------------------------------
	// 分離軸を割り出す
	std::vector<Vector3> separatingAxises;
	separatingAxises.insert(separatingAxises.end(), obb1SurfaceNormals.begin(), obb1SurfaceNormals.end());
	separatingAxises.insert(separatingAxises.end(), obb2SurfaceNormals.begin(), obb2SurfaceNormals.end());
	separatingAxises.insert(separatingAxises.end(), crossSeparatingAxises.begin(), crossSeparatingAxises.end());

	// obbから頂点を取り出す
	std::vector<Vector3> obb1Indecies = colliderA.MakeIndex();
	std::vector<Vector3> obb2Indecies = colliderB.MakeIndex();

	// 頂点を分離軸候補に射影したベクトルを格納する

	// 取り出した頂点を分離軸へ射影する
	for (uint8_t axis = 0; axis < separatingAxises.size(); axis++) {
		std::vector<float> obb1ProjectIndecies;
		std::vector<float> obb2ProjectIndecies;

		for (uint8_t oi = 0; oi < obb1Indecies.size(); oi++) {
			// 各obbの頂点を射影する
			// 正射影ベクトルの長さを求める
			obb1ProjectIndecies.push_back(Dot(obb1Indecies[oi], separatingAxises[axis]));
			obb2ProjectIndecies.push_back(Dot(obb2Indecies[oi], separatingAxises[axis]));
		}

		// 最大値/最小値を取り出す
		float maxObb1 = *std::max_element(obb1ProjectIndecies.begin(), obb1ProjectIndecies.end());
		float maxObb2 = *std::max_element(obb2ProjectIndecies.begin(), obb2ProjectIndecies.end());
		float minObb1 = *std::min_element(obb1ProjectIndecies.begin(), obb1ProjectIndecies.end());
		float minObb2 = *std::min_element(obb2ProjectIndecies.begin(), obb2ProjectIndecies.end());

		// 影の長さを得る
		float projectLenght1 = float(maxObb1 - minObb1);
		float projectLenght2 = float(maxObb2 - minObb2);

		float sumSpan = projectLenght1 + projectLenght2;
		float longSpan = ((std::max)(maxObb1, maxObb2) - ((std::min)(minObb1, minObb2)));

		// 影の長さの合計 < 2つの影の両端の差分だったら分離軸が存在しているためfalse
		if (sumSpan <= longSpan) {
			return false;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　AABBとAABB
//////////////////////////////////////////////////////////////////////////////////////////////////

bool CheckCollision(const AABB& aabb1, const AABB& aabb2) {
	if (aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) {

	} else {
		return false;
	}

	if (aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) {

	} else {
		return false;
	}

	if (aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z) {

	} else {
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　AABBと球の当たり判定
//////////////////////////////////////////////////////////////////////////////////////////////////

bool CheckCollisionAABBandSphere(const AABB& aabb, const Sphere& sphere) {
	// 最近傍点を求める
	Vector3 closestPoint{
		std::clamp(sphere.center.x, aabb.min.x, aabb.max.x),
		std::clamp(sphere.center.y, aabb.min.y, aabb.max.y),
		std::clamp(sphere.center.z, aabb.min.z, aabb.max.z)
	};

	// 最近傍点ト球の中心の距離を求める
	float distance = Length(closestPoint - sphere.center);

	// 距離が半径よりも小さければ衝突
	if (distance <= sphere.radius) {
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　OBBと球の当たり判定
//////////////////////////////////////////////////////////////////////////////////////////////////

bool CheckCollisionOBBandSphere(const OBB& obb, const Sphere& sphere) {
	// 回転行列を作成する
	Matrix4x4 rotateMatrix = obb.matRotate;
	// 平行移動分を作成
	Matrix4x4 matTranslate = obb.center.MakeTranslateMat();
	// ワールド行列を作成
	Matrix4x4 obbMatWorld = rotateMatrix * matTranslate;
	Matrix4x4 obbMatWorldInverse = obbMatWorld.Inverse();

	// 中心点を作成
	Vector3 centerInOBBLocal = Transform(sphere.center, obbMatWorldInverse);

	// OBBからABBを作成
	AABB aabbOBBLocal{ .min = obb.size * -1, .max = obb.size };
	Sphere sphereOBBLocal{ .center = centerInOBBLocal, .radius = sphere.radius};

	// ローカル空間で衝突判定
	if (CheckCollisionAABBandSphere(aabbOBBLocal, sphereOBBLocal)) {
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　OBBとAABBの当たり判定
//////////////////////////////////////////////////////////////////////////////////////////////////

bool CheckCollisionAABBandOBB(const OBB& obb, const AABB& aabb) {
	AABB aabbOBBLocal{ .min = obb.size * -1, .max = obb.size };
	if (CheckCollision(aabbOBBLocal, aabb)) {
		return true;
	}

	return false;
}

//================================================================================================//
//								当たり判定の呼び出し関数群											　//
//================================================================================================//

bool CheckCollision(const AABB& aabb, const Sphere& sphere) {
	return CheckCollisionAABBandSphere(aabb, sphere);
}
bool CheckCollision(const Sphere& sphere, const AABB& aabb) {
	return CheckCollisionAABBandSphere(aabb, sphere);
}

bool CheckCollision(const OBB& obb, const Sphere& sphere) {
	return CheckCollisionOBBandSphere(obb, sphere);
}
bool CheckCollision(const Sphere& sphere, const OBB& obb) {
	return CheckCollisionOBBandSphere(obb, sphere);
}

bool CheckCollision(const OBB& obb, const AABB& aabb) {
	return CheckCollisionAABBandOBB(obb, aabb);
}
bool CheckCollision(const AABB& aabb, const OBB& obb) {
	return CheckCollisionAABBandOBB(obb, aabb);
}

bool CheckCollision(const std::variant<Sphere, AABB, OBB>& shape1, const std::variant<Sphere, AABB, OBB>& shape2) {
	return std::visit(
		[](const auto& lhs, const auto& rhs) {
			return CheckCollision(lhs, rhs); // 各組み合わせの CheckCollision を呼び出す
		},
		shape1, shape2);
}
