#include "BoxCollider.h"

bool IsCollision(const AABB& aabb, const Vector3& point) {
    Vector3 closestPoint{
		std::clamp(point.x, aabb.min.x, aabb.max.x),
		std::clamp(point.y, aabb.min.y, aabb.max.y),
		std::clamp(point.z, aabb.min.z, aabb.max.z)
	};

	// 最近傍点ト球の中心の距離を求める
	float distance = Length(closestPoint - point);

	// 距離が半径よりも小さければ衝突
	if (distance <= 0.01f) {
		return true;
	}

	return false;
}
