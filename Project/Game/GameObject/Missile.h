#pragma once
#include "Game/GameObject/CanThrowObject.h"
#include "Engine/Math/MyRandom.h"
#include "Engine/Math/MyMath.h"

/// <summary>
/// ミサイルオブジェクト
/// </summary>
class Missile : public CanThrowObject {
public:

	Missile();
	~Missile();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

public:

	void Pop(const Vector3& targePos, const Vector3& firePos);

#ifdef _DEBUG
	void Debug_Draw();
#endif

private:

	void OnCollisionEnter([[maybe_unused]] MeshCollider& other);
	void OnCollisionStay([[maybe_unused]] MeshCollider& other);
	void OnCollisionExit([[maybe_unused]] MeshCollider& other);

public:

	MeshCollider* GetMeshCollider() { return meshCollider_.get(); }

	// 生存フラグの取得・設定
	const bool GetIsAlive() const { return isAlive_; }
	void SetIsAlive(bool isAlive) { isAlive_ = isAlive; }

private:

	static const uint32_t kDivision_ = 50;

	Vector3 controlPoint_[3];
	std::vector<Vector3> movePoint_;

	bool isAlive_;

	float moveT_;
	float nextMoveT_;
	float speed_;

	// -------------------------------------------------
	// ↓ 他クラスの情報
	// -------------------------------------------------
};

