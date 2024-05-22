#pragma once
#include "Matrix4x4.h"

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

	// =============================================
	/// <summary>
	/// 加算
	/// </summary>
	/// <param name="obj"></param>
	/// <returns></returns>
	Vector3 operator+(const Vector3& obj) const { return Vector3(x + obj.x, y + obj.y, z + obj.z); }

	Vector3 operator+(const float& obj) const { return Vector3(x + obj, y + obj, z + obj); }

	Vector3 operator+=(const Vector3& obj) {
		x += obj.x;
		y += obj.y;
		z += obj.z;
		return *this;
	}

	Vector3 operator+=(const float& obj) {
		x += obj;
		y += obj;
		z += obj;
		return *this;
	}

	/// <summary>
	/// 減算
	/// </summary>
	/// <param name="obj"></param>
	/// <returns></returns>
	Vector3 operator-(const Vector3& obj) const { return Vector3(x - obj.x, y - obj.y, z - obj.z); }

	Vector3 operator-(const float& obj) const { return Vector3(x - obj, y - obj, z - obj); }

	Vector3 operator-=(const Vector3& obj) {
		x -= obj.x;
		y -= obj.y;
		z -= obj.z;
		return *this;
	}

	Vector3 operator-=(const float& obj) {
		x -= obj;
		y -= obj;
		z -= obj;
		return *this;
	}

	/// <summary>
	/// 乗算
	/// </summary>
	/// <param name="obj"></param>
	/// <returns></returns>
	Vector3 operator*(const Vector3& obj) const { return Vector3(x * obj.x, y * obj.y, z * obj.z); }

	Vector3 operator*(const float& obj) const { return Vector3(x * obj, y * obj, z * obj); }

	Vector3 operator*=(const Vector3& obj) {
		x *= obj.x;
		y *= obj.y;
		z *= obj.z;
		return *this;
	}

	Vector3 operator*=(const float& obj) {
		x *= obj;
		y *= obj;
		z *= obj;
		return *this;
	}

	// Matrix
	Vector3 operator*(const Matrix4x4& mat) {
		Vector3 result{};
		result.x = mat.m[0][0] * x + mat.m[0][1] * y + mat.m[0][2] * z + mat.m[0][3];
		result.y = mat.m[1][0] * x + mat.m[1][1] * y + mat.m[1][2] * z + mat.m[1][3];
		result.z = mat.m[2][0] * x + mat.m[2][1] * y + mat.m[2][2] * z + mat.m[2][3];

		return result;
	}

};