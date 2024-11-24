#pragma once
#include "Engine/GameObject/BaseGameObject.h"

class Bomb : public BaseGameObject {
public:

	Bomb();
	~Bomb();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void Pop(const Vector3& pos, const Vector3& acceleration = Vector3::ZERO());

#ifdef _DEBUG
	void Debug_Draw();
#endif
public:

	MeshCollider* GetMeshCollider() { return meshCollider_.get(); }

	// 生存フラグの取得・設定
	const bool GetIsAlive() const { return isAlive_; }
	void SetIsAlive(bool isAlive) { isAlive_ = isAlive; }

private:

	void OnCollisionEnter([[maybe_unused]] MeshCollider& other);
	void OnCollisionStay([[maybe_unused]] MeshCollider& other);
	void OnCollisionExit([[maybe_unused]] MeshCollider& other);

private:

	bool isAlive_ = false;

	Vector3 velocity_;
	Vector3 acceleration_;
};

