#pragma once

struct Vector4 final {
	float x;
	float y;
	float z;
	float w;

	// =============================================
	/// <summary>
	/// 加算
	/// </summary>
	/// <param name="obj"></param>
	/// <returns></returns>
	Vector4 operator+(const Vector4& obj) const { return Vector4(x + obj.x, y + obj.y, z + obj.z); }

	Vector4 operator+(const float& obj) const { return Vector4(x + obj, y + obj, z + obj); }

	Vector4 operator+=(const Vector4& obj) {
		x += obj.x;
		y += obj.y;
		z += obj.z;
		return *this;
	}

	Vector4 operator+=(const float& obj) {
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
	Vector4 operator-(const Vector4& obj) const { return Vector4(x - obj.x, y - obj.y, z - obj.z); }

	Vector4 operator-(const float& obj) const { return Vector4(x - obj, y - obj, z - obj); }

	Vector4 operator-=(const Vector4& obj) {
		x -= obj.x;
		y -= obj.y;
		z -= obj.z;
		return *this;
	}

	Vector4 operator-=(const float& obj) {
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
	Vector4 operator*(const Vector4& obj) const { return Vector4(x * obj.x, y * obj.y, z * obj.z); }

	Vector4 operator*(const float& obj) const { return Vector4(x * obj, y * obj, z * obj); }

	Vector4 operator*=(const Vector4& obj) {
		x *= obj.x;
		y *= obj.y;
		z *= obj.z;
		return *this;
	}

	Vector4 operator*=(const float& obj) {
		x *= obj;
		y *= obj;
		z *= obj;
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