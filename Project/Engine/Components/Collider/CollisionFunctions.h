#pragma once
#include "Engine/Lib/Math/MathStructures.h"
#include <variant>

//================================================================================================//
//									当たり判定の関数群												　//
//================================================================================================//

/// <summary>///
/// 球同士の当たり判定
/// </summary>///
/// <param name="s1">球1</param>///
/// <param name="s2">球2</param>///
/// <returns></returns>
bool CheckCollision(const Sphere& s1, const Sphere& s2);

/// <summary>
/// OBB同士の当たり判定
/// </summary>
/// <param name="colliderA">: OBBA</param>
/// <param name="colliderB">: OBBB</param>
/// <returns></returns>
bool CheckCollison(const OBB& colliderA, const OBB& colliderB);

/// <summary>
/// 当たり判定
/// </summary>
/// <param name="aabb1">箱1</param>
/// <param name="aabb2">箱2</param>
/// <returns>true: 当たっている false: 当たっていない</returns>
bool CheckCollision(const AABB& aabb1, const AABB& aabb2);

/// <summary>
/// 球との当たり判定
/// </summary>
/// <param name="aabb">箱</param>
/// <param name="sphere">球</param>
/// <returns>true: 当たっている false: 当たっていない</returns>
bool CheckCollisionAABBandSphere(const AABB& aabb, const Sphere& sphere);

/// <summary>
/// OBBと球の当たり判定
/// </summary>
///  <param name="obb"></param>
/// <param name="sphere"></param>
///  <returns></returns>
bool CheckCollisionOBBandSphere(const OBB& obb, const Sphere& sphere);

/// <summary>
/// OBBとAABBの当たり判定
/// </summary>
/// <param name="obb"></param>
/// <param name="aabb"></param>
/// <returns></returns>
bool CheckCollisionAABBandOBB(const OBB& obb, const AABB& aabb);


//================================================================================================//
//								当たり判定の呼び出し関数群											　//
//================================================================================================//

bool CheckCollision(const AABB& aabb, const Sphere& sphere);
bool CheckCollision(const Sphere& sphere, const AABB& aabb);

bool CheckCollision(const OBB& obb, const Sphere& sphere);
bool CheckCollision(const Sphere& sphere, const OBB& obb);

bool CheckCollision(const OBB& obb, const AABB& aabb);
bool CheckCollision(const AABB& aabb, const OBB& obb);

bool CheckCollision(const std::variant<Sphere, AABB, OBB>& shape1,
                    const std::variant<Sphere, AABB, OBB>& shape2);