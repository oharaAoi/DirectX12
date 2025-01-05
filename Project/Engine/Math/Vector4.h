#pragma once
#include "Engine/Math/Matrix4x4.h"
#include "Engine/Math/Vector3.h"

class Vector4 final {
public:

	float x;
	float y;
	float z;
	float w;

	Vector4() = default;
	~Vector4() = default;

	Vector4(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w) {
	}

	Vector4(const Vector3& vec3, float w = 1.0f)
		: x(vec3.x), y(vec3.y), z(vec3.z), w(w) {
	}

	// =============================================
	/// <summary>
	/// 加算
	/// </summary>
	/// <param name="obj"></param>
	/// <returns></returns>
	Vector4 operator+(const Vector4& obj) const { return Vector4(x + obj.x, y + obj.y, z + obj.z, w + obj.w); }

	Vector4 operator+(const float& obj) const { return Vector4(x + obj, y + obj, z + obj, w + obj); }

	Vector4 operator+=(const Vector4& obj) {
		x += obj.x;
		y += obj.y;
		z += obj.z;
		w += obj.w;
		return *this;
	}

	Vector4 operator+=(const float& obj) {
		x += obj;
		y += obj;
		z += obj;
		w += obj;
		return *this;
	}

	/// <summary>
	/// 減算
	/// </summary>
	/// <param name="obj"></param>
	/// <returns></returns>
	Vector4 operator-(const Vector4& obj) const { return Vector4(x - obj.x, y - obj.y, z - obj.z, w - obj.w); }

	Vector4 operator-(const float& obj) const { return Vector4(x - obj, y - obj, z - obj, w - obj); }

	Vector4 operator-=(const Vector4& obj) {
		x -= obj.x;
		y -= obj.y;
		z -= obj.z;
		w -= obj.w;
		return *this;
	}

	Vector4 operator-=(const float& obj) {
		x -= obj;
		y -= obj;
		z -= obj;
		w -= obj;
		return *this;
	}

	/// <summary>
	/// 乗算
	/// </summary>
	/// <param name="obj"></param>
	/// <returns></returns>
	Vector4 operator*(const Vector4& obj) const { return Vector4(x * obj.x, y * obj.y, z * obj.z, w * obj.w); }

	Vector4 operator*(const float& obj) const { return Vector4(x * obj, y * obj, z * obj, w * obj); }

	Vector4 operator*=(const Vector4& obj) {
		x *= obj.x;
		y *= obj.y;
		z *= obj.z;
		w *= obj.w;
		return *this;
	}

	Vector4 operator*=(const float& obj) {
		x *= obj;
		y *= obj;
		z *= obj;
		w *= obj;
		return *this;
	}

	// Matrix
	Vector4 operator*(const Matrix4x4& mat) {
		Vector4 result{};
		result.x = mat.m[0][0] * x + mat.m[0][1] * y + mat.m[0][2] * z + mat.m[0][4];
		result.y = mat.m[1][0] * x + mat.m[1][1] * y + mat.m[1][2] * z + mat.m[1][4];
		result.z = mat.m[2][0] * x + mat.m[2][1] * y + mat.m[2][2] * z + mat.m[2][4];

		return result;
	}
};