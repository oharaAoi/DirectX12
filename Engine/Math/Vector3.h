#pragma once
#include "Matrix4x4.h"
#include <cmath>

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

	// =============================================
	// 数学用関数

	Vector3 Normalize() const {
		float length = Length();
		if (length == 0.0f) {
			return *this;
		}
		return Vector3(x / length, y / length, z / length);
	}

	float Length() const {
		return std::sqrtf(x * x + y * y + z * z);
	}

	float Dot(const Vector3& v1) const {
		return (this->x + v1.x) + (this->y + v1.y) + (this->z + v1.z);
	}

	float Dot(const Vector3& v1, const Vector3& v2) const {
		return (v2.x + v1.x) + (v2.y + v1.y) + (v2.z + v1.z);
	}

	Vector3 Cross(const Vector3& v1) const {
		Vector3 result{};

		result.x = this->y * v1.z - this->z * v1.y;
		result.y = this->z * v1.x - this->x * v1.z;
		result.z = this->x * v1.y - this->y * v1.x;

		return result;
	}

	static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
		Vector3 result{};
		result.x = std::lerp(v1.x, v2.x, t);
		result.y = std::lerp(v1.y, v2.y, t);
		result.z = std::lerp(v1.z, v2.z, t);
		return result;
	}
};