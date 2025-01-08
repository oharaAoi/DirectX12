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

Matrix3x3 Vector2::MakeScaleMat(const Vector2& scale) {
	Matrix3x3 result{};
	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = 1;

	return result;
}

Matrix3x3 Vector2::MakeRotateMat(float theta) {
	Matrix3x3 result{};
	result.m[0][0] = cosf(theta);
	result.m[0][1] = sinf(theta);
	result.m[1][0] = -sinf(theta);
	result.m[1][1] = cosf(theta);
	result.m[2][2] = 1;

	return result;
}

Matrix3x3 Vector2::MakeTranslateMat(const Vector2& translate) {
	Matrix3x3 result{};
	result.m[0][0] = 1;
	result.m[1][1] = 1;
	result.m[2][0] = translate.x;
	result.m[2][1] = translate.y;
	result.m[2][2] = 1;

	return result;
}

Matrix3x3 Vector2::MakeAffine(const Vector2& scale, float theta, const Vector2& translate) {
	Matrix3x3 scaleMatrix{};
	Matrix3x3 rotateMatrix{};
	Matrix3x3 translateMatrix{};
	Matrix3x3 worldMatrix{};

	scaleMatrix = MakeScaleMat(scale);
	rotateMatrix = MakeRotateMat(theta);
	translateMatrix = MakeTranslateMat(translate);

	worldMatrix = Matrix3x3::Multiply(Matrix3x3::Multiply(scaleMatrix, rotateMatrix), translateMatrix);

	return worldMatrix;
}