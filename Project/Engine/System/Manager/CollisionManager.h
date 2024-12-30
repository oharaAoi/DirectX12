#pragma once
#include <list>
#include "Engine/Assets/Collider/ICollider.h"

/// <summary>
/// Collisionの判定を取る
/// </summary>
class CollisionManager {
public:

	CollisionManager();
	~CollisionManager();

	void Init();
	
	/// <summary>
	/// すべての当たり判定チェック
	/// </summary>
	void CheckAllCollision();

	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA">コライダーAQ</param>
	/// <param name="colliderB">コライダーB</param>
	void CheckCollisionPair(ICollider* colliderA, ICollider* colliderB);

	/// <summary>
	/// コライダーの追加
	/// </summary>
	/// <param name="collider"></param>
	void AddCollider(ICollider* collider) { colliders_.push_back(collider); };

	// リストを空にする
	void Reset() { 
		colliders_.clear();
	}

private:

	std::list<ICollider*> colliders_;
	
};

