#include "Quaternion.h"

Quaternion Quaternion::Normalize() const {
	float length = std::sqrt(x * x + y * y + z * z + w * w);
	if (length < 1e-6f) { // 長さが非常に小さい場合
		return Quaternion(0, 0, 0, 1); // デフォルト値を返す
	}
	float invLength = 1.0f / length;
	return Quaternion(x * invLength, y * invLength, z * invLength, w * invLength);
}

Matrix4x4 Quaternion::MakeMatrix() const {
	// クォータニオンの各成分
	Matrix4x4 result = Matrix4x4::MakeUnit();
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

Quaternion Quaternion::AngleAxis(float angle, const Vector3& axis) {
	Vector3 normalizeAxis = axis.Normalize();
	//float sinHalfAngle = std::sin(angle / 2.0f);

	return Quaternion(
		normalizeAxis.x * std::sin(angle / 2.0f),
		normalizeAxis.y * std::sin(angle / 2.0f),
		normalizeAxis.z * std::sin(angle / 2.0f),
		std::cos(angle / 2.0f)
	);
}

Quaternion Quaternion::EulerToQuaternion(const Vector3& euler) {
	// オイラー角をラジアンに変換（角度の場合）
	float halfPitch = euler.x * 0.5f;
	float halfYaw = euler.y * 0.5f;
	float halfRoll = euler.z * 0.5f;

	// 三角関数の計算
	float cosPitch = std::cos(halfPitch);
	float sinPitch = std::sin(halfPitch);
	float cosYaw = std::cos(halfYaw);
	float sinYaw = std::sin(halfYaw);
	float cosRoll = std::cos(halfRoll);
	float sinRoll = std::sin(halfRoll);

	// クォータニオンの計算
	return Quaternion(
		cosYaw * sinPitch * cosRoll + sinYaw * cosPitch * sinRoll, // x
		sinYaw * cosPitch * cosRoll - cosYaw * sinPitch * sinRoll, // y
		cosYaw * cosPitch * sinRoll - sinYaw * sinPitch * cosRoll,  // z
		cosYaw * cosPitch * cosRoll + sinYaw * sinPitch * sinRoll // w
	).Normalize();
}

Quaternion Quaternion::FromToRotation(const Vector3& fromDire, const Vector3& toDire) {
	Vector3 cross = Vector3::Cross(fromDire, toDire);  // クロス積
	float dot = Vector3::Dot(fromDire, toDire);        // ドット積

	// 回転角度
	float angle = std::acos(dot);

	// 回転軸
	if (cross.x == 0 && cross.y == 0 && cross.z == 0) {
		cross = Vector3(fromDire.y, -fromDire.x, 0.0f);
	}
	Vector3 axis = cross.Normalize();

	// クォータニオンを計算
	float sinHalfAngle = std::sin(angle / 2.0f);
	float cosHalfAngle = std::cos(angle / 2.0f);

	return Quaternion(axis.x * sinHalfAngle, axis.y * sinHalfAngle, axis.z * sinHalfAngle, cosHalfAngle);
}

Quaternion Quaternion::Inverse(const Quaternion& rotation) {
	float norm = rotation.w * rotation.w + rotation.x * rotation.x + rotation.y * rotation.y + rotation.z * rotation.z;
	return Quaternion(-rotation.x / norm, -rotation.y / norm, -rotation.z / norm, rotation.w / norm);
}

Quaternion Quaternion::LookRotation(const Vector3& forward, const Vector3& upVector) {
	// 正面からforwardに向ける回転を取得
	Quaternion lookRotation = FromToRotation(Vector3{ 0.0f, 0.0f, 1.0f }, forward.Normalize());
	// 外積を用いてupVectorとforwardに垂直なベクトルを求める
	Vector3 xAxisHorizontal = Cross(upVector, forward).Normalize();
	if (xAxisHorizontal.Length() < 1e-6) {
		// forwardとupVectorが平行な場合、回転が不定になるため例外処理やデフォルト回転を返す
		return lookRotation;
	}
	// 回転後のy軸を求める
	Vector3 yAxisAfterRotate = Cross(forward, xAxisHorizontal).Normalize();
	// Look後のy軸から回転後のy軸へ修正する回転を求める
	Vector3 yAxisVBefore = (lookRotation * Vector3{ 0.0f, 1.0f, 0.0f }).Normalize();
	Quaternion modifyRotation = FromToRotation(yAxisVBefore, yAxisAfterRotate);
	return modifyRotation * lookRotation;
}

Quaternion Quaternion::LookAt(const Vector3& from, const Vector3& to, const Vector3& up) {
	Vector3 forward = (to - from).Normalize();
	return LookRotation(forward, up);
}

float Quaternion::Dot(const Quaternion& q1, const Quaternion& q2) {
	return (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) + (q1.w * q2.w);
}

Quaternion Quaternion::Slerp(const Quaternion& q1, const Quaternion& q2, const float& t) {
	float dot = Dot(q1, q2);
	dot = std::clamp(dot, -1.0f, 1.0f);

	Quaternion newQ1 = q1;
	if (dot < 0.0f) {
		newQ1 = Quaternion(-q1.x, -q1.y, -q1.z, -q1.w);
		dot = -dot;
	}

	if (dot > 0.9995f) { // 線形補間 (Lerp) を使用
		Quaternion lerpResult = Quaternion(
			(1.0f - t) * newQ1.x + t * q2.x,
			(1.0f - t) * newQ1.y + t * q2.y,
			(1.0f - t) * newQ1.z + t * q2.z,
			(1.0f - t) * newQ1.w + t * q2.w
		);
		return lerpResult.Normalize(); // 正規化
	}

	float rad = std::acos(dot); // 内積に基づいて角度を取得
	float bottom = std::sin(rad); // sinθ
	if (std::fabs(bottom) < 1e-6f) {
		Quaternion lerpResult = Quaternion(
			(1.0f - t) * newQ1.x + t * q2.x,
			(1.0f - t) * newQ1.y + t * q2.y,
			(1.0f - t) * newQ1.z + t * q2.z,
			(1.0f - t) * newQ1.w + t * q2.w
		);
		return lerpResult.Normalize(); // 正規化
	}

	float a_rotate = std::sin((1.0f - t) * rad) / bottom;
	float b_rotate = std::sin(t * rad) / bottom;

	Quaternion result = Quaternion(
		(newQ1.x * a_rotate) + (q2.x * b_rotate),
		(newQ1.y * a_rotate) + (q2.y * b_rotate),
		(newQ1.z * a_rotate) + (q2.z * b_rotate),
		(newQ1.w * a_rotate) + (q2.w * b_rotate)
	);

	return result.Normalize();
}

Quaternion Quaternion::EulerToQuaternion(float pitch, float yaw, float roll) {
	// オイラー角 (pitch, yaw, roll) をラジアンに変換
	float halfPitch = pitch * 0.5f;
	float halfYaw = yaw * 0.5f;
	float halfRoll = roll * 0.5f;

	// 三角関数の計算
	float cosPitch = std::cos(halfPitch);
	float sinPitch = std::sin(halfPitch);
	float cosYaw = std::cos(halfYaw);
	float sinYaw = std::sin(halfYaw);
	float cosRoll = std::cos(halfRoll);
	float sinRoll = std::sin(halfRoll);

	// クォータニオンの計算
	return Quaternion(
		cosYaw * sinPitch * cosRoll + sinYaw * cosPitch * sinRoll, // x
		sinYaw * cosPitch * cosRoll - cosYaw * sinPitch * sinRoll, // y
		cosYaw * cosPitch * sinRoll - sinYaw * sinPitch * cosRoll,  // z
		cosYaw * cosPitch * cosRoll + sinYaw * sinPitch * sinRoll // w
	);
}

Vector3 Quaternion::QuaternionToEuler() const {
	Vector3 euler;

	// Roll (Z軸回転)
	float sinr_cosp = 2.0f * (w * z + x * y);
	float cosr_cosp = 1.0f - 2.0f * (z * z + x * x);
	euler.z = std::atan2(sinr_cosp, cosr_cosp);

	// Pitch (X軸回転)
	float sinp = 2.0f * (w * x - y * z);
	if (std::abs(sinp) >= 1) {
		euler.x = std::copysign(3.14159265f / 2.0f, sinp); // クランプ
	} else{
		euler.x = std::asin(sinp);
	}
	// Yaw (Y軸回転)
	float siny_cosp = 2.0f * (w * y + z * x);
	float cosy_cosp = 1.0f - 2.0f * (x * x + y * y);
	euler.y = std::atan2(siny_cosp, cosy_cosp);

	return euler;
}

Quaternion Quaternion::ToQuaternion(const Vector4& v) {
	return Quaternion(v.x, v.y, v.z, v.w);
}

Vector3 Quaternion::MakeForward() const {
	float xx = x * x;
	float yy = y * y;
	float xz = x * z;
	float yz = y * z;
	float wx = w * x;
	float wy = w * y;
	return  Vector3(2.0f * (xz + wy), 2.0f * (yz - wx), 1.0f - (2.0f * (xx + yy))).Normalize();
}

Vector3 Quaternion::MakeUp() const {
	float xx = x * x;
	float zz = z * z;
	float xy = x * y;
	float yz = y * z;
	float wx = w * x;
	float wz = w * z;
	return Vector3(2.0f * (xy - wz), 1 - (2.0f * (xx + zz)), 2.0f * (yz + wx)).Normalize();
}

Vector3 Quaternion::MakeRight() const {
	// 標準的な右方向ベクトル (1, 0, 0)
	Vector3 right(1.0f, 0.0f, 0.0f);
	Quaternion result = (*this);

	return (result * right);
}

Vector3 Quaternion::ToEulerAngles() const {
	Vector3 eulerAngles;
	// ピッチ (X軸の回転)
	float sinp = 2.0f * (w * x + y * z);
	if (std::abs(sinp) >= 1.0f)
		eulerAngles.x = std::copysign(PI / 2.0f, sinp); // ピッチが ±90度 の場合
	else
		eulerAngles.x = std::asin(sinp);

	// ヨー (Y軸の回転)
	float siny_cosp = 2.0f * (w * y - z * x);
	float cosy_cosp = 1.0f - 2.0f * (x * x + y * y);
	eulerAngles.y = std::atan2(siny_cosp, cosy_cosp);

	// ロール (Z軸の回転)
	float sinr_cosp = 2.0f * (w * z + x * y);
	float cosr_cosp = 1.0f - 2.0f * (y * y + z * z);
	eulerAngles.z = std::atan2(sinr_cosp, cosr_cosp);

	return eulerAngles;
}

Quaternion Quaternion::Conjugate() const {
	return Quaternion(-x, -y, -z, w);
}

Vector3 Quaternion::Rotate(const Vector3& vec) const {
	Quaternion qVec(vec.x, vec.y, vec.z, 0); // ベクトルをクォータニオンとして表現
	Quaternion qConj = Conjugate(); // クォータニオンの共役を取る
	Quaternion qResult = *this * qVec * qConj; // クォータニオンによる回転
	return Vector3(qResult.x, qResult.y, qResult.z); // 回転したベクトルを返す
}

Quaternion Quaternion::operator*(const Quaternion& q2) const {
	Vector3 v1 = Vector3(this->x, this->y, this->z);
	Vector3 v2 = Vector3(q2.x, q2.y, q2.z);

	float dot = Vector3::Dot(v1, v2);
	float newW = (this->w * q2.w) - dot;

	Vector3 cross = Vector3::ZERO();
	cross.x = v1.y * v2.z - v1.z * v2.y;
	cross.y = v1.z * v2.x - v1.x * v2.z;
	cross.z = v1.x * v2.y - v1.y * v2.x;

	Vector3 vector = cross + (v1 * q2.w) + (v2 * this->w);
	return Quaternion(vector.x, vector.y, vector.z, newW);
}
//
//Vector3 Quaternion::operator*(const Vector3& v) {
//	Quaternion vecQuaternion = Quaternion(v.x, v.y, v.z, 0.0f);
//	Quaternion vecAfterRotate = (*this) * vecQuaternion * Inverse((*this)).Normalize();
//	return Vector3(vecAfterRotate.x, vecAfterRotate.y, vecAfterRotate.z);
//}

Vector3 Quaternion::operator*(const Vector3& v) const {
	Vector3 qVec{ x, y, z };
	Vector3 t = Vector3::Cross(qVec, v) * 2.0f;
	return v + t * w + Vector3::Cross(qVec, t);
}


