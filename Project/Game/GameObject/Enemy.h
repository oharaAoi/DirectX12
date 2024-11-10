#pragma once
#include <memory>
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Collider/MeshCollider.h"

enum class EnemyType {
	STAR,
	JET
};

class Enemy
	: public BaseGameObject {
public:

	Enemy();
	~Enemy();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void OnCollision(MeshCollider& other);

	void ChangeModel();

#ifdef _DEBUG
	void Debug_Gui();
#endif

	MeshCollider* GetMeshCollider() { return meshCollider_.get(); }

	const Vector3 GetWorldPos() const;
	const Vector3 GetTranslate() const { return transform_->GetTranslation(); }

	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }
	const Vector3 GetVelocity() const { return velocity_; }

	const bool GetIsAlive() const { return isAlive_; }

	const bool GetIsMove() const { return isMove_; }
	void SetIsMove(bool isMove) { isMove_ = isMove; }

	void SetEnemyType(const EnemyType type) { enemyType_ = type; }
	const EnemyType GetEnemyType() const { return enemyType_; }
	
private:

	std::unique_ptr<MeshCollider> meshCollider_;

	EnemyType enemyType_;
	EnemyType preEnemyType_;

	Vector3 velocity_;

	bool isAlive_;
	bool isMove_;

	float speed_;
};

