#pragma once
#include <array>
#include "Engine.h"
#include "Engine/Lib/Math/Vector3.h"
#include "Engine/Lib/Math/Quaternion.h"
#include "Engine/Lib/Math/MyMatrix.h"
#include "Engine/Lib/Math/MathStructures.h"

/// <summary>
/// グリット線を描画する
/// </summary>
/// <param name="viewProjectionMatrix"></param>
void DrawGrid(const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix);

/// <summary>
/// 球を描画する
/// </summary>
/// <param name="center">: 球の中心点</param>
/// <param name="radius">: 球の半径</param>
/// <param name="viewProjectionMatrix">: viewProjectionMatrix</param>
void DrawSphere(const Vector3& center, float radius, const Matrix4x4& viewProjectionMatrix, const Vector4& color = { 1,1,1,1 });

/// <summary>
/// Cone形状を描画
/// </summary>
/// <param name="center"></param>
/// <param name="radius"></param>
/// <param name="angle"></param>
/// <param name="height"></param>
/// <param name="viewProjectionMatrix"></param>
void DrawCone(const Vector3& center, const Quaternion& rotate, float radius, float angle, float height, const Matrix4x4& viewProjectionMatrix);

/// <summary>
/// AABBを描画する
/// </summary>
/// <param name="aabb"></param>
/// <param name="viewMatrix"></param>
/// <param name="projectionMatrix"></param>
void DrawAABB(const AABB& aabb, const Matrix4x4& vpMatrix, const Vector4& color = { 1,1,1,1 });

/// <summary>
/// OBBを描画する
/// </summary>
/// <param name="aabb"></param>
/// <param name="viewMatrix"></param>
/// <param name="projectionMatrix"></param>
void DrawOBB(const OBB& obb, const Matrix4x4& vpMatrix, const Vector4& color = { 1,1,1,1 });