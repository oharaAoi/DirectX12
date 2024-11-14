#pragma once
#include <list>
#include "Engine/Collider/Collider.h"
#include "Engine/Collider/MeshCollider.h"
#include "Engine/Math/Matrix4x4.h"
#include "Engine/Utilities/DrawUtils.h"

/// <summary>
/// Collisionの判定を取る
/// </summary>
class CollisionManager {
public:

	CollisionManager();
	~CollisionManager();

	void Init();
	//void Draw(const Matrix4x4& vpMat) const;

	/// <summary>
	/// すべての当たり判定チェック
	/// </summary>
	void CheckAllCollision();

	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA">コライダーAQ</param>
	/// <param name="colliderB">コライダーB</param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

	/// <summary>
	/// Obbでの当たり判定を行う
	/// </summary>
	/// <param name="colliderA"></param>
	/// <param name="colliderB"></param>
	bool CheckCollisonObb(Collider* colliderA, Collider* colliderB);

	/// <summary>
	/// コライダーの追加
	/// </summary>
	/// <param name="collider"></param>
	void AddCollider(Collider* collider) { colliders_.push_back(collider); };

	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA">コライダーAQ</param>
	/// <param name="colliderB">コライダーB</param>
	void CheckCollisionPair(MeshCollider* colliderA, MeshCollider* colliderB);

	/// <summary>
	/// Obbでの当たり判定を行う
	/// </summary>
	/// <param name="colliderA"></param>
	/// <param name="colliderB"></param>
	bool CheckCollisonObb(MeshCollider* colliderA, MeshCollider* colliderB);

	/// <summary>
	/// コライダーの追加
	/// </summary>
	/// <param name="collider"></param>
	void AddCollider(MeshCollider* collider) { meshColliders_.push_back(collider); };

	// リストを空にする
	void Reset() { 
		colliders_.clear();
		meshColliders_.clear();
	}

	void SetPlayerPosition(const Vector3& pos) { playerPosition_ = pos; }

private:

	std::list<Collider*> colliders_;
	std::list<MeshCollider*> meshColliders_;
	std::list<MeshCollider*> preMeshColliders_;

	Vector3 playerPosition_;
	
};

