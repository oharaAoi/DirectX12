#pragma once
#include "Engine/GameObject/BaseGameObject.h"

class CanThrowObject : public BaseGameObject {
public:

	CanThrowObject() = default;
	virtual ~CanThrowObject() = default;

	void Finalize() override = 0;
	void Init() override = 0;
	void Update() override = 0;
	void Draw() const override = 0;

	/// <summary>
	/// 投げる処理の前準備
	/// </summary>
	/// <param name="velocity"></param>
	void ReadyToThrow(const Vector3& velocity);

	// 速度の取得・設定
	const Vector3 GetVelocity() const { return velocity_; }
	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }

	// 捕まえられたかのフラグの取得・設定
	const bool GetIsWireCaught() const { return isWireCaught_; }
	void SetIsWireCaught(bool isWireCaught) { isWireCaught_ = isWireCaught; }

	// 投げられたかのフラグ
	const bool GetIsThrowed() const { return isThrowed_; }
	void SetIsThrowed(bool isThrowed) { isThrowed_ = isThrowed; }

protected:

	Vector3 velocity_;

	bool isThrowed_;	// 投げられたか
	bool isWireCaught_;	// ワイヤーに捕まえられたか

};

