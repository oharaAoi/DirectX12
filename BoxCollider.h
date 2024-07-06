#pragma once
#include "MyMath.h"
#include "MathStructures.h"

/// <summary>
/// 点との当たり判定
/// </summary>
/// <param name="aabb">箱</param>
/// <param name="point">点</param>
/// <returns>ture : 当たっている　false : 当たっていない</returns>
bool IsCollision(const AABB& aabb, const Vector3& point);
