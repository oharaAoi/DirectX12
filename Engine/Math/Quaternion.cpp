#include "Quaternion.h"

Quaternion Quaternion::Normalize() const {
	float num = std::sqrtf(Dot(*this, *this));
	if (num < kEpsilon) {
		return Quaternion();
	}
	return Quaternion(this->x / num, this->y / num, this->z / num, this->w / num);
}

Matrix4x4 Quaternion::MakeMatrix() const {
	// クォータニオンの各成分
	Matrix4x4 result;
	float xx = x * x;
	float yy = y * y;
	float zz = z * z;
	float ww = w * w;
	float xy = x * y;
	float xz = x * z;
	float yz = y * z;
	float wx = w * x;
	float wy = w * y;
	float wz = w * z;

	result.m[0][0] = ww + xx - yy - zz;
	result.m[0][1] = 2.0f * (xy + wz);
	result.m[0][2] = 2.0f * (xz - wy);
	result.m[0][3] = 0.0f;

	result.m[1][0] = 2.0f * (xy - wz);
	result.m[1][1] = ww - xx + yy - zz;
	result.m[1][2] = 2.0f * (yz + wx);
	result.m[1][3] = 0.0f;

	result.m[2][0] = 2.0f * (xz + wy);
	result.m[2][1] = 2.0f * (yz - wx);
	result.m[2][2] = ww - xx - yy + zz;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

Quaternion Quaternion::AngleAxis(const float& angle, const Vector3& axis) {
	Vector3 normalizeAxis = axis.Normalize();
	// 度数法から孤度法に変更
	float rad = angle * toRadian;

	return Quaternion(
		normalizeAxis.x * std::sinf(rad / 2.0f),
		normalizeAxis.y * std::sinf(rad / 2.0f),
		normalizeAxis.z * std::sinf(rad / 2.0f),
		std::cosf(rad / 2.0f)
	);
}

Quaternion Quaternion::FromToRotation(const Vector3& fromDire, const Vector3& toDire) {
	// 外積を用いて軸ベクトルを求める
	Vector3 axis = Cross(fromDire, toDire);
	// 外積が0の時は無回転のquaternionにする
	if (axis.x == 0 && axis.y == 0 && axis.z == 0) {
		return Quaternion();
	}

	// 内積の定義から回転量を求める 
	float rad = std::acosf(fromDire.Dot(toDire) / (fromDire.Length() * toDire.Length()));

	return AngleAxis(rad * toDegree, axis);
}

Quaternion Quaternion::Inverse(const Quaternion& rotation) {
	return Quaternion(-rotation.x, -rotation.y, -rotation.z, -rotation.w);
}

Quaternion Quaternion::LookRotation(const Vector3& forward, const Vector3& upVector) {
	// 正面からforwardに向ける回転を取得
	Quaternion lookRotation = FromToRotation(Vector3{ 0.0f,0.0f,1.0f }, forward);
	// 外積を用いてupVectorとforwardに垂直なベクトルを求める
	Vector3 xAxisHorizontal = Cross(upVector, forward);
	// 回転後のy軸を求める
	Vector3 yAxisAfterRotate = Cross(forward, xAxisHorizontal);

	// Look後のy軸から回転後のy軸へ修正する回転を求める
	Vector3 yAxisVBefore = lookRotation * Vector3{ 0.0f, 1.0f, 0.0f };
	Quaternion modifyRotation = FromToRotation(yAxisVBefore, yAxisAfterRotate);

	return modifyRotation * lookRotation;
}

float Quaternion::Dot(const Quaternion& q1, const Quaternion& q2) {
	return (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) + (q1.w * q2.w);
}

Quaternion Quaternion::Slerp(const Quaternion& q1, const Quaternion& q2, const float& t) {
	// 2つの回転の内積を求める
	float dot = Dot(q1, q2);
	dot = std::clamp(dot, -1.0f, 1.0f);

	Quaternion newQ2 = q2;
	// 内積が負の時最短距離で補完を得るため片方の回転を負にして内積を正にする
	if (dot < 0) {
		newQ2 = Quaternion(-q2.x, -q2.y, -q2.z, -q2.w).Normalize();
		dot = -dot;
	}

	// 角度が0に近い場合は線形補間
	if (dot > 0.9995f) {
		return Quaternion(
			q1.x + t * (newQ2.x - q1.x),
			q1.y + t * (newQ2.y - q1.y),
			q1.z + t * (newQ2.z - q1.z),
			q1.w + t * (newQ2.w - q1.w)
		).Normalize();
	}

	// 二つの回転の角度を求める
	float rad = std::acosf(dot);
	float bottom = std::sinf(rad);
	float a_rotate = std::sinf((1 - t) * rad) / bottom;
	float b_rotate = std::sinf(t * rad) / bottom;

	Quaternion result = Quaternion(
		q1.x * a_rotate + newQ2.x * b_rotate,
		q1.y * a_rotate + newQ2.y * b_rotate,
		q1.z * a_rotate + newQ2.z * b_rotate,
		q1.w * a_rotate + newQ2.w * b_rotate
	);

	return result.Normalize();
}

Quaternion Quaternion::operator*(const Quaternion& q2) const {
	return Quaternion(
		(this->w * q2.x) + (this->x * q2.w) + (this->y * q2.z) - (this->z * q2.y),
		(this->w * q2.y) - (this->x * q2.z) + (this->y * q2.w) + (this->z * q2.x),
		(this->w * q2.z) + (this->x * q2.y) - (this->y * q2.x) + (this->z * q2.w),
		(this->w * q2.w) - (this->x * q2.x) - (this->y * q2.y) - (this->z * q2.z)
	);
}

Vector3 Quaternion::operator*(const Vector3& v) {
	// ベクトルをクォータニオンに変換（w = 0）
	Quaternion vecQuaternion(v.x, v.y, v.z, 0.0f);
	// クォータニオンの共役を取得
	Quaternion conjugate = Quaternion(-this->x, -this->y, -this->z, this->w);
	// 回転後のベクトルクォータニオンを計算
	Quaternion vecAfterRotate = (*this) * vecQuaternion * conjugate;
	// 回転後のベクトルを返す
	return Vector3(vecAfterRotate.x, vecAfterRotate.y, vecAfterRotate.z);
}


