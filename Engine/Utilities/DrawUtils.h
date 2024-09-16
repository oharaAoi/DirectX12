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
/// AABBを描画する
/// </summary>
/// <param name="aabb"></param>
/// <param name="viewMatrix"></param>
/// <param name="projectionMatrix"></param>
void DrawAABB(const AABB& aabb, const Matrix4x4& vpMatrix, const Vector4& color);