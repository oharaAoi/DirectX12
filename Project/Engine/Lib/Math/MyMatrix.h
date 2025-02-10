#pragma once
#include <Engine/Lib/Math/Matrix3x3.h>
#include <Engine/Lib/Math/Matrix4x4.h>
#include <Engine/Lib/Math/Vector3.h>
#include <assert.h>
#include <cmath>

class Quaternion;

/// <summary>
/// 加算
/// </summary>
/// <param name="m1"></param>
/// <param name="m2"></param>
/// <returns></returns>
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);

/// <summary>
/// 減算
/// </summary>
/// <param name="m1"></param>
/// <param name="m2"></param>
/// <returns></returns>
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);

/// <summary>
/// 積
/// </summary>
/// <param name="m1"></param>
/// <param name="m2"></param>
/// <returns></returns>
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

/// <summary>
/// 逆行列
/// </summary>
/// <param name="m"></param>
/// <returns></returns>
Matrix4x4 Inverse(Matrix4x4 matrix);

void swapRows(Matrix4x4& matrix, int row1, int row2);
void scaleRow(Matrix4x4& matrix, int row, float scalar);
void addScaledRow(Matrix4x4& matrix, int targetRow, int sourceRow, float scalar);

/// <summary>
/// 座標変換
/// </summary>
/// <param name="vector"></param>
/// <param name="matrix"></param>
/// <returns></returns>
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

Vector3 GetEulerAnglesFromRotationMat(const Matrix4x4& R);