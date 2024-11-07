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
	Quaternion q;
	q.x = cosYaw * sinPitch * cosRoll - sinYaw * cosPitch * sinRoll;
	q.y = sinYaw * cosPitch * cosRoll + cosYaw * sinPitch * sinRoll;
	q.z = cosYaw * cosPitch * sinRoll - sinYaw * sinPitch * cosRoll;
	q.w = cosYaw * cosPitch * cosRoll + sinYaw * sinPitch * sinRoll;

	return q.Normalize();  // 正規化して返す
}

Quaternion Quaternion::FromToRotation(const Vector3& fromDire, const Vector3& toDire) {
	// 外積を用いて軸ベクトルを求める
	Vector3 axis = Cross(fromDire, toDire);
	// 外積が0の時は無回転のquaternionにする
	if (axis.x == 0 && axis.y == 0 && axis.z == 0) {
		return Quaternion();
	}

	// 内積の定義から回転量を求める 
	float rad = std::acosf(Vector3::Dot(fromDire, toDire) / (fromDire.Length() * toDire.Length()));

	return AngleAxis(rad * toDegree, axis);

	//Vector3 fromNorm = fromDire.Normalize();
	//Vector3 toNorm = toDire.Normalize();

	//// 回転軸を求める（外積）
	//Vector3 axis = Vector3::Cross(fromNorm, toNorm);

	//// 回転角度を求める（内積からコサインを取得）
	//float dot = Vector3::Dot(fromNorm, toNorm);
	//float angle = std::acos(dot);

	//// 角度をもとに四元数を計算
	//float sinHalfAngle = std::sin(angle / 2);
	//float cosHalfAngle = std::cos(angle / 2);

	//return Quaternion{
	//	axis.x * sinHalfAngle,
	//	axis.y * sinHalfAngle,
	//	axis.z * sinHalfAngle,
	//	cosHalfAngle,
	//};
}

Quaternion Quaternion::Inverse(const Quaternion& rotation) {
	float norm = rotation.w * rotation.w + rotation.x * rotation.x + rotation.y * rotation.y + rotation.z * rotation.z;
	return Quaternion(-rotation.x / norm, -rotation.y / norm, -rotation.z / norm, rotation.w / norm);
}

Quaternion Quaternion::LookRotation(const Vector3& forward, const Vector3& upVector) {
	Vector3 z = forward.Normalize();
	Vector3 x = Vector3::Cross(upVector, z).Normalize();
	Vector3 y = Vector3::Cross(z, x);

	// 3x3 回転行列の要素
	float m00 = x.x, m01 = x.y, m02 = x.z;
	float m10 = y.x, m11 = y.y, m12 = y.z;
	float m20 = z.x, m21 = z.y, m22 = z.z;

	// 回転行列から四元数を生成
	float trace = m00 + m11 + m22;
	if (trace > 0) {
		float s = 0.5f / std::sqrt(trace + 1.0f);
		return Quaternion{
			0.25f / s,
			(m21 - m12) * s,
			(m02 - m20) * s,
			(m10 - m01) * s
		};
	} else if (m00 > m11 && m00 > m22) {
		float s = 2.0f * std::sqrt(1.0f + m00 - m11 - m22);
		return Quaternion{
			(m21 - m12) / s,
			0.25f * s,
			(m01 + m10) / s,
			(m02 + m20) / s
		};
	} else if (m11 > m22) {
		float s = 2.0f * std::sqrt(1.0f + m11 - m00 - m22);
		return Quaternion{
			(m02 - m20) / s,
			(m01 + m10) / s,
			0.25f * s,
			(m12 + m21) / s
		};
	} else {
		float s = 2.0f * std::sqrt(1.0f + m22 - m00 - m11);
		return Quaternion{
			(m10 - m01) / s,
			(m02 + m20) / s,
			(m12 + m21) / s,
			0.25f * s
		};
	}
}

Quaternion Quaternion::LookAt(const Vector3& from, const Vector3& to) {
	// プレイヤーから敵への方向ベクトルを計算
	Vector3 direction = (to - from).Normalize();

	// 現在の前方ベクトル（z方向を前方と仮定）
	Vector3 forward(0.0f, 0.0f, 1.0f);

	// 回転軸を計算（外積を利用）
	Vector3 rotationAxis = Vector3::Cross(forward, direction);
	if (rotationAxis.x == 0 && rotationAxis.y == 0 && rotationAxis.z == 0) {
		// forward と direction が同じか逆向きの場合、適切なクォータニオンを返す
		return Quaternion(1, 0, 0, 0);
	}

	rotationAxis = rotationAxis.Normalize();

	// 内積から角度を計算
	float angle = std::acos(Vector3::Dot(forward, direction));

	// 回転クォータニオンを作成
	return Quaternion::AngleAxis(angle, rotationAxis);
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
		/*newQ2 = Quaternion(-q2.x, -q2.y, -q2.z, -q2.w);*/
		dot = -dot;
	}

	// 角度が0に近い場合は線形補間
	if (dot > 0.9995f) {
		return Quaternion(
			q1.x + t * (newQ2.x - q1.x),
			q1.y + t * (newQ2.y - q1.y),
			q1.z + t * (newQ2.z - q1.z),
			q1.w + t * (newQ2.w - q1.w)
		);
	}

	// 二つの回転の角度を求める
	float rad = std::acos(dot);
	float bottom = std::sin(rad);
	float a_rotate = std::sin((1.0f - t) * rad) / bottom;
	float b_rotate = std::sin(t * rad) / bottom;

	Quaternion result = Quaternion(
		(q1.x * a_rotate) + (newQ2.x * b_rotate),
		(q1.y * a_rotate) + (newQ2.y * b_rotate),
		(q1.z * a_rotate) + (newQ2.z * b_rotate),
		(q1.w * a_rotate) + (newQ2.w * b_rotate)
	);

	return result;
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

Quaternion Quaternion::operator*(const Quaternion& q2) const {
	Vector3 v1 = Vector3(this->x, this->y, this->z);
	Vector3 v2 = Vector3(q2.x, q2.y, q2.z);

	float dot = Vector3::Dot(v1, v2);
	float newW = (this->w * q2.w) - dot;

	Vector3 cross{};
	cross.x = v1.y * v2.z - v1.z * v2.y;
	cross.y = v1.z * v2.x - v1.x * v2.z;
	cross.z = v1.x * v2.y - v1.y * v2.x;

	Vector3 vector = cross + (v1 * q2.w) + (v2 * this->w);
	return Quaternion(vector.x, vector.y, vector.z, newW);
}

Vector3 Quaternion::operator*(const Vector3& v) {
	Quaternion vecQuaternion = Quaternion(v.x, v.y, v.z, 0.0f);
	Quaternion vecAfterRotate = (*this) * vecQuaternion * Inverse((*this)).Normalize();
	return Vector3(vecAfterRotate.x, vecAfterRotate.y, vecAfterRotate.z);
}


