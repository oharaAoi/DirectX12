#pragma once
#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Vector4.h"
#include "Engine/Math/MyMatrix.h"
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <vector>
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

////////////////////////////////////////////////////////////////////////////////////
// 線形補完
////////////////////////////////////////////////////////////////////////////////////

/// <summary>
/// 線形補完
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <returns></returns>
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

/// <summary>
/// 線形補完
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <returns></returns>
Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);

/// <summary>
/// 最短角度を求める
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <param name="t"></param>
/// <returns></returns>
float LerpShortAngle(const float& originalAngle, const float& targetAngle, const float& t);

/// <summary>/// 
/// 角度を求める
/// </summary>///
///  <param name="v1"></param>/// 
/// <param name="v2"></param>///
///  <returns></returns>
float Angle(const Vector3& v1, const Vector3& v2);


/// <summary>/// 
/// CatmullRom補完
/// </summary>/// 
/// <param name="p0">点0の座標</param>/// 
/// <param name="p1">点1の座標</param>/// 
/// <param name="p2">点2の座標</param>/// 
/// <param name="p3">点3の座標</param>/// 
/// <param name="t">点1と点2の間で指定された座標</param>/// 
/// <returns>点1と点2の間で指定された座標</returns>
Vector3 CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t);

/// <summary>/// 
/// CatmullRomスプライン曲線上の座標を得る
/// </summary>///
/// <param name="points">制御点の集合</param>/// 
/// <param name="t">スプライン全区間の中で割合指定</param>/// 
/// <returns>座標</returns>
Vector3 CatmullRomPosition(const std::vector<Vector3>& points, float t);

/// <summary>
/// ベジエ曲線を書く
/// </summary>
///  <param name="controlPoint">制御点がまとまった配列</param>
///  <returns></returns>
Vector3 Bezier(const std::vector<Vector3>& controlPoint, const float& t);
//
///// <summary>
///// ベジエ曲線を書く
///// </summary>
///// <param name="controlPoint">制御点がまとまった配列</param>
///// <returns></returns>
//Vector3 Bezier(const Vector3& v1, const Vector3& v2, const Vector3& v3, const float& t);


uint32_t Red(uint32_t color);
uint32_t Green(uint32_t color);
uint32_t Blue(uint32_t color);
uint32_t Alpha(uint32_t color);
Vector4 FloatColor(uint32_t color);
uint32_t IntColor(const Vector4& color);
uint32_t HSV_to_RGB(float h, float s, float v, float alpha);