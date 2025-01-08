#pragma once
#include <array>
#include "Engine.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/MyMatrix.h"
#include "Engine/Math/MathStructures.h"

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
void DrawSphere(const Vector3& center, float radius, const Matrix4x4& viewProjectionMatrix);

/// <summary>
/// AABBを描画する
/// </summary>
/// <param name="aabb"></param>
/// <param name="viewMatrix"></param>
/// <param name="projectionMatrix"></param>
void DrawAABB(const AABB& aabb, const Matrix4x4& vpMatrix, const Vector4& color);

/// <summary>
/// OBBを描画する
/// </summary>
/// <param name="aabb"></param>
/// <param name="viewMatrix"></param>
/// <param name="projectionMatrix"></param>
void DrawOBB(const OBB& obb, const Vector4& color);


void DrawMatrix(const Matrix4x4& mat, const std::string& id);

void DrawQuaternion(const Quaternion& q, const std::string& id);

void DrawVector(const Vector3& vector, const std::string& id);