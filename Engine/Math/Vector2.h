#pragma once

struct Vector2 final {
	float x;
	float y;

	// =============================================
	/// <summary>
	/// 加算
	/// </summary>
	/// <param name="obj"></param>
	/// <returns></returns>
	Vector2 operator+(const Vector2& obj) const { return Vector2(x + obj.x, y + obj.y); }

	Vector2 operator+(const float& obj) const { return Vector2(x + obj, y + obj); }

	Vector2 operator+=(const Vector2& obj) {
		x += obj.x;
		y += obj.y;
		return *this;
	}

	Vector2 operator+=(const float& obj) {
		x += obj;
		y += obj;
		return *this;
	}

	/// <summary>
	/// 減算
	/// </summary>
	/// <param name="obj"></param>
	/// <returns></returns>
	Vector2 operator-(const Vector2& obj) const { return Vector2(x - obj.x, y - obj.y); }

	Vector2 operator-(const float& obj) const { return Vector2(x - obj, y - obj); }

	Vector2 operator-=(const Vector2& obj) {
		x -= obj.x;
		y -= obj.y;
		return *this;
	}

	Vector2 operator-=(const float& obj) {
		x -= obj;
		y -= obj;
		return *this;
	}

	/// <summary>
	/// 乗算
	/// </summary>
	/// <param name="obj"></param>
	/// <returns></returns>
	Vector2 operator*(const Vector2& obj) const { return Vector2(x * obj.x, y * obj.y); }

	Vector2 operator*(const float& obj) const { return Vector2(x * obj, y * obj); }

	Vector2 operator*=(const Vector2& obj) {
		x *= obj.x;
		y *= obj.y;
		return *this;
	}

	Vector2 operator*=(const float& obj) {
		x *= obj;
		y *= obj;
		return *this;
	}
};