#pragma once
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Math/MyRandom.h"
#include "Engine/Math/MyMath.h"
#include "Game/GameObject/Player.h"

/// <summary>
/// ミサイルオブジェクト
/// </summary>
class Missile : public BaseGameObject {
public:

	Missile();
	~Missile();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

public:

	void Pop(const Vector3& targePos, const Vector3& firePos);

private:

	void OnCollisionEnter([[maybe_unused]] MeshCollider& other);
	void OnCollisionStay([[maybe_unused]] MeshCollider& other);
	void OnCollisionExit([[maybe_unused]] MeshCollider& other);

public:

	MeshCollider* GetMeshCollider() { return meshCollider_.get(); }

	// 生存フラグの取得・設定
	const bool GetIsAlive() const { return isAlive_; }
	void SetIsAlive(bool isAlive) { isAlive_ = isAlive; }

	// 捕まえられたかのフラグの取得・設定
	const bool GetIsWireCaught() const { return isWireCaught_; }
	void SetIsWireCaught(bool isWireCaught) { isWireCaught_ = isWireCaught; }

private:

	static const uint32_t kDivision_ = 50;

	Vector3 controlPoint_[3];
	std::vector<Vector3> movePoint_;

	bool isAlive_;
	bool isWireCaught_;	// ワイヤーに捕まえられたか
	bool isThrowed_;	// 投げられたか

	float moveT_;
	float speed_;

	Vector3 velocity_;

	// -------------------------------------------------
	// ↓ 他クラスの情報
	// -------------------------------------------------
};

