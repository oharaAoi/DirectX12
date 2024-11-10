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

Quaternion Quaternion::LookAt(const Vector3& from, const Vector3& to, const Vector3& fromForward) {
	// ターゲットまでの方向ベクトルを計算
	Vector3 direction = (to - from).Normalize();

	// XZ平面でのヨー回転
	Vector3 forwardXZ(direction.x, 0.0f, direction.z);
	forwardXZ = forwardXZ.Normalize();
	float yawAngle = std::atan2(forwardXZ.x, forwardXZ.z);

	// YZ平面でのピッチ回転
	float pitchAngle = std::asin(-direction.y); // 単純にy成分からピッチを計算

	// ヨー回転（XZ平面）
	Quaternion yawQuat = Quaternion::AngleAxis(yawAngle, Vector3(0.0f, 1.0f, 0.0f)); // Y軸周りの回転
	// ピッチ回転（YZ平面）
	Quaternion pitchQuat = Quaternion::AngleAxis(pitchAngle, Vector3(1.0f, 0.0f, 0.0f)); // X軸周りの回転

	// ヨーとピッチの回転を組み合わせたクォータニオン
	return yawQuat * pitchQuat;
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

float Quaternion::GetYawFromQuaternion(const Quaternion& q) {
	return std::atan2f(2.0f * (q.w * q.z + q.x * q.y), 1.0f - 2.0f * (q.y * q.y + q.z * q.z));
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

Quaternion Quaternion::operator+(const Quaternion& q) const {
	return Quaternion(x + q.x, y + q.y, z + q.z, w + q.w);
}

Quaternion Quaternion::operator-(const Quaternion& q) const {
	return Quaternion(x - q.x, y - q.y, z - q.z, w - q.w);
}

Quaternion Quaternion::operator*(float scalar) const {
	return Quaternion(x * scalar, y * scalar, z * scalar, w * scalar);
}

#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
void Quaternion::Debug_Gui(Quaternion& rotate, const char* id) {
	Quaternion debug_rotate;
	ImGui::DragFloat4(id, &debug_rotate.x, 0.01f);
	rotate = (rotate.Normalize() * debug_rotate.Normalize());
}
#endif

/// <summary>
/// Quaternion版のCatmullRom補完
/// </summary>
/// <param name="q0">Quaternion 0</param>
/// <param name="q1">Quaternion 1</param>
/// <param name="q2">Quaternion 2</param>
/// <param name="q3">Quaternion 3</param>
/// <param name="t">0と1の間の補完率</param>
/// <returns>補完されたQuaternion</returns>
Quaternion Quaternion::CatmullRomInterRotate(const Quaternion& q0, const Quaternion& q1, const Quaternion& q2, const Quaternion& q3, float t) {
	const float s = 0.5f; // 数式に出てくる1/2

	float t2 = t * t; // tの2乗
	float t3 = t2 * t; // tの3乗

	// ベクトル値関数の式
	Quaternion e3 = (q0 * -1) + (q1 * 3) - (q2 * 3) + q3;
	Quaternion e2 = (q0 * 2) - (q1 * 5) + (q2 * 4) - q3;
	Quaternion e1 = (q0 * -1) + q2;
	Quaternion e0 = q1 * 2;

	// SLERPを用いて補間
	Quaternion result = ((e3 * t3) + (e2 * t2) + (e1 * t) + e0) * s;
	return result;
}

/// <summary>
/// Quaternion版のCatmullRomスプライン曲線上のクォータニオン
/// </summary>
/// <param name="points">クォータニオンの制御点</param>
/// <param name="t">スプラインの全体に対する割合指定</param>
/// <returns>内挿されたQuaternion</returns>
Quaternion Quaternion::CatmullRomRotate(const std::vector<Quaternion>& points, float t) {
	assert(points.size() >= 4 && "制御点は4以上必要です");
	// 区間数は制御点の数の-1
	size_t division = points.size() - 1;
	// 1区間の長さ(全体を1.0とした割合)
	float areaWidth = 1.0f / division;

	// 区間の始点を0.0f, 終点を1.0fとしたときの現在の位置
	float t2 = std::fmod(t, areaWidth) / areaWidth;
	t2 = std::clamp(t2, 0.0f, 1.0f);

	// 区間番号
	size_t index = static_cast<size_t>(t / areaWidth);
	// 区間番号が上限を超えないように収める
	index = std::clamp(static_cast<int>(index), 0, static_cast<int>(division) - 1);

	// 4点分のインデックス
	size_t index0 = index - 1;
	size_t index1 = index;
	size_t index2 = index + 1;
	size_t index3 = index + 2;

	// 始点の場合はp0とp1は同じ
	if (index == 0) {
		index0 = index1;
	}
	// 最後の区間はp3とp2は同じ
	if (index3 >= points.size()) {
		index3 = index2;
	}

	// 4点のクォータニオン
	const Quaternion& q0 = points[index0];
	const Quaternion& q1 = points[index1];
	const Quaternion& q2 = points[index2];
	const Quaternion& q3 = points[index3];

	return CatmullRomInterRotate(q0, q1, q2, q3, t2);
}