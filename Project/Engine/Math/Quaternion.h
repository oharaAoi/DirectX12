#pragma once
#include "Engine/Math/MyMath.h"

class Quaternion {
public:


	Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {};
	Quaternion(const float& _x, const float& _y, const float& _z, const float& _w) :
		x(_x), y(_y), z(_z), w(_w) {};

	~Quaternion() {};


	/// <summary>
	/// 正規化した回転を返す
	/// </summary>
	/// <returns></returns>
	Quaternion Normalize() const;

	Matrix4x4 MakeMatrix() const;

	Vector3 MakeForward() const;
	Vector3 MakeUp() const;
	Vector3 MakeRight() const;

	/// <summary>
	/// Quaternionからオイラー角
	/// </summary>
	/// <returns></returns>
	Vector3 ToEulerAngles() const;

	Quaternion Conjugate() const;
public:

	/// <summary>
	/// axisの周りをangle度回転するQuaternionを生成する
	/// </summary>
	/// <param name="angle">: 回転させる角度</param>
	/// <param name="axis">: 回転させる軸</param>
	/// <returns></returns>
	static Quaternion AngleAxis(float angle, const Vector3& axis);

	/// <summary>
	/// オイラー角をクォータニオンに変換
	/// </summary>
	/// <param name="euler"></param>
	/// <returns></returns>
	static Quaternion EulerToQuaternion(const Vector3& euler);

	/// <summary>
	/// fromからtoへの回転を返す
	/// </summary>
	/// <param name="fromDire"></param>
	/// <param name="toDire"></param>
	/// <returns></returns>
	static Quaternion FromToRotation(const Vector3& fromDire, const Vector3& toDire);

	/// <summary>
	/// 逆クォータニオンを生成
	/// </summary>
	/// <param name="rotation"></param>
	/// <returns></returns>
	static Quaternion Inverse(const Quaternion& rotation);

	/// <summary>
	/// 向きたい方向の回転を求める関数
	/// </summary>
	/// <param name="forward"></param>
	/// <param name="upVector"></param>
	/// <returns></returns>
	static Quaternion LookRotation(const Vector3& forward, const Vector3& upVector);

	/// <summary>
	/// fromから見たtoへの回転を生成する
	/// </summary>
	/// <param name="from"></param>
	/// <param name="to"></param>
	/// <returns></returns>
	static Quaternion LookAt(const Vector3& from, const Vector3& to);

	/// <summary>
	/// 進行方向を向くクォータニオンを生成
	/// </summary>
	/// <param name="direction">: 方向</param>
	/// <param name="up">: UP方向のベクトル</param>
	/// <returns></returns>
	static Quaternion FromDirection(const Vector3& direction, const Vector3& up = Vector3::UP());
	
	/// <summary>
	/// 二つの回転の内積を返す
	/// </summary>
	/// <param name="q1"></param>
	/// <param name="q2"></param>
	/// <returns></returns>
	static float Dot(const Quaternion& q1, const Quaternion& q2);

	/// <summary>
	/// 球面線形補間
	/// </summary>
	/// <param name="q1"></param>
	/// <param name="q2"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, const float& t);
	
public:

	Quaternion operator*(const Quaternion& q2) const;
	Vector3 operator*(const Vector3& v);
	
public:

	float x;
	float y;
	float z;
	float w;

};