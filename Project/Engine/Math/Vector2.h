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

    // ベクトルの長さ
    float Length() const {
        return std::sqrt((x * x) + (y * y));
    }

    Vector2 Normalize() const {
        Vector2 result;
        float len = this->Length();
        if (len != 0) {
            result.x /= len;
            result.y /= len;
        }

        return result;
    }

    static Vector2 Lerp(const Vector2& st, const Vector2& end, float t) {
        return {
            std::lerp(st.x, end.x, t),
            std::lerp(st.y, end.y, t)
        };
    }
};