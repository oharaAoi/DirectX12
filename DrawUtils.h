#pragma once
#include "Engine.h"
#include "MyMatrix.h"

/// <summary>
/// グリット線を描画する
/// </summary>
/// <param name="viewProjectionMatrix"></param>
void DrawGrid(const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix);

