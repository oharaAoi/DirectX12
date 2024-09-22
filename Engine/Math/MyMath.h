#pragma once
#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Vector4.h"
#include "Engine/Math/MyMatrix.h"
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <numbers>
#include <limits>

static const float PI = std::numbers::pi_v<float>;
static const float toRadian = std::numbers::pi_v<float> / 180.0f;
static const float toDegree = 180.0f / std::numbers::pi_v<float>;

static const float kEpsilon = std::numeric_limits<float>::epsilon();

float Length(const Vector3& vec3);

Vector3 Normalize(const Vector3& vec3);

/// <summary>
/// 内積
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
float Dot(const Vector3& v1, const Vector3& v2);

/// <summary>
/// クロス積
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 Cross(const Vector3& v1, const Vector3& v2);

/// <summary>
/// スクリーン座標からワールド座標に変換する関数
/// </summary>
/// <param name="screenPos">スクリーン上の座標</param>
/// <param name="inverseWvpMat">逆ワールドプロジェクション行列</param>
/// <param name="setDirection">ワールド上のどこに設定するか</param>
/// <returns>ワールド座標</returns>
Vector3 ScreenToWorldCoordinate(const Vector2& screenPos, const Matrix4x4& inverseWvpMat, const float& setDirection);

/// <summary>
/// ベクトル変換
/// </summary>
/// <param name="v"></param>
/// <param name="m"></param>
/// <returns></returns>
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

