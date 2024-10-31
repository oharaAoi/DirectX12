#include "Vector2.h"

float Vector2::Length() const {
	return std::sqrt((x * x) + (y * y));
}

Vector2 Vector2::Normalize() const {
	Vector2 result = { x, y };
	float len = this->Length();
	if (len != 0) {
		 result.x /= len;
		 result.y /= len;
	}
	return result;
}

float Vector2::Dot(const Vector2& v1, const Vector2& v2) {
	return ((v1.x * v2.x) + (v1.y * v2.y));
}

float Vector2::Cross(const Vector2& v1, const Vector2& v2) {
	return (v1.x * v2.y) - (v2.x * v1.y);
}

Vector2 Vector2::Lerp(const Vector2& st, const Vector2& end, float t) {
	return {
		std::lerp(st.x, end.x, t),
		std::lerp(st.y, end.y, t)
	};
}
