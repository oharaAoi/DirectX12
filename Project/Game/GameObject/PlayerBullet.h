#pragma once
#include <memory>
#include "Engine/GameObject/BaseGameObject.h"
#include "Game/GameObject/Player.h"
#include "Game/UI/KnockDownEnemy.h"
#include "Engine/Collider/MeshCollider.h"

class PlayerBullet : public BaseGameObject {
public:

	PlayerBullet();
	~PlayerBullet();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void OnCollision(MeshCollider& other);

#ifdef _DEBUG
	void Debug_Gui();
#endif

	MeshCollider* GetMeshCollider() { return meshCollider_.get(); }

	const Vector3 GetWorldPos() const;

	void SetPopPos(const Vector3& pos);

	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }

	void SetIsAlive(bool isAlive) { isAlive_ = isAlive; }
	const bool GetIsAlive() const { return isAlive_; }

private:

	Vector3 velocity_;

	float speed_;
	
	bool isAlive_;

	std::unique_ptr<MeshCollider> meshCollider_;
};
