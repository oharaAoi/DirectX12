#pragma once
#include <cmath>

class Vector2 final {
public:
	float x;
	float y;

	// コンストラクタ
	Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

	// 加算
	Vector2 operator+(const Vector2& obj) const { return Vector2(x + obj.x, y + obj.y); }
	Vector2 operator+(float obj) const { return Vector2(x + obj, y + obj); }

	// 加算代入
	Vector2& operator+=(const Vector2& obj) {
		x += obj.x;
		y += obj.y;
		return *this;
	}
	Vector2& operator+=(float obj) {
		x += obj;
		y += obj;
		return *this;
	}

	// 減算
	Vector2 operator-(const Vector2& obj) const { return Vector2(x - obj.x, y - obj.y); }
	Vector2 operator-(float obj) const { return Vector2(x - obj, y - obj); }

	// 減算代入
	Vector2& operator-=(const Vector2& obj) {
		x -= obj.x;
		y -= obj.y;
		return *this;
	}
	Vector2& operator-=(float obj) {
		x -= obj;
		y -= obj;
		return *this;
	}

	// 乗算
	Vector2 operator*(const Vector2& obj) const { return Vector2(x * obj.x, y * obj.y); }
	Vector2 operator*(float obj) const { return Vector2(x * obj, y * obj); }

	// 乗算代入
	Vector2& operator*=(const Vector2& obj) {
		x *= obj.x;
		y *= obj.y;
		return *this;
	}
	Vector2& operator*=(float obj) {
		x *= obj;
		y *= obj;
		return *this;
	}

	// =============================================
	// 数学用関数
	// =============================================

	/// <summary>
	/// ベクトルの長さ
	/// </summary>
	/// <returns></returns>
	float Length() const;

	/// <summary>
	/// 正規化
	/// </summary>
	/// <returns></returns>
	Vector2 Normalize() const;

	/// <summary>
	/// 内積
	/// </summary>
	/// <param name="v1">: ベクトル1</param>
	/// <param name="v2">: ベクトル2</param>
	/// <returns></returns>
	static float Dot(const Vector2& v1, const Vector2& v2);

	/// <summary>
	/// 外積
	/// </summary>
	/// <param name="v1">: ベクトル1</param>
	/// <param name="v2">: ベクトル2</param>
	/// <returns></returns>
	static float Cross(const Vector2& v1, const Vector2& v2);

	/// <summary>
	/// 線形補完
	/// </summary>
	/// <param name="st">: 開始位置</param>
	/// <param name="end">: 終了位置</param>
	/// <param name="t">: 係数</param>
	/// <returns></returns>
	static Vector2 Lerp(const Vector2& st, const Vector2& end, float t);
};