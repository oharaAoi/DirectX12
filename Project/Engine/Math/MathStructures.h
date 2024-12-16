#pragma once
#include <vector>
#include "MyMath.h"
#include "Engine/Math/Quaternion.h"

/// <summary>
/// 拡縮、回転、移動
/// </summary>
struct SRT {
	Vector3 scale;
	Quaternion rotate;
	Vector3 translate;
};

/// <summary>
/// 球
/// </summary>
struct Sphere {
	Vector3 center;
	float radius;
};

/// <summary>
/// 軸平行境界箱
/// </summary>
struct AABB {
	Vector3 min;
	Vector3 max;
};

/// <summary>
/// OBB(有向境界箱)
/// </summary>
struct OBB {
	Vector3 center; // 中心点
	Vector3 orientations[3]; // 座標軸、正規化、直交必須
	Vector3 size; // 座標軸方向の長さの半分

	// 回転軸
	Matrix4x4 matRotate;

	void MakeOBBAxis(const Quaternion& rotate);
	std::vector<Vector3> MakeIndex() const;
};
