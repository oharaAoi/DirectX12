#include "CollisionManager.h"
#include "Engine/Assets/Collider/CollisionFunctions.h"

CollisionManager::CollisionManager() {}
CollisionManager::~CollisionManager() {}


/////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void CollisionManager::Init() {
	colliders_.clear();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　すべての当たり判定チェック
//////////////////////////////////////////////////////////////////////////////////////////////////

void CollisionManager::CheckAllCollision() {

	// リスト内のペアの総当たり判定
	std::list<ICollider*>::iterator iterA = colliders_.begin();
	for (; iterA != colliders_.end(); ++iterA) {
		ICollider* colliderA = *iterA;

		// イテレータBはイテレータAの次の要素から回す
		std::list<ICollider*>::iterator iterB = iterA;
		iterB++;

		for (; iterB != colliders_.end(); ++iterB) {
			ICollider* colliderB = *iterB;
			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　コライダー2つの衝突判定と応答
//////////////////////////////////////////////////////////////////////////////////////////////////

void CollisionManager::CheckCollisionPair(ICollider* colliderA, ICollider* colliderB) {
	if (CheckCollision(colliderA->GetShape(), colliderB->GetShape())) {
		// Colliderの状態を変化させる
		colliderA->SwitchCollision(colliderB);
		colliderB->SwitchCollision(colliderA);

		colliderA->OnCollision(*colliderB);
		colliderB->OnCollision(*colliderA);
	} else {
		// 衝突している状態だったら脱出した状態にする
		if (colliderA->GetCollisionState() == CollisionFlags::STAY) {
			colliderA->SetCollisionState(CollisionFlags::EXIT);
			colliderA->OnCollision(*colliderB);
		} else {
			colliderA->SetCollisionState(CollisionFlags::NONE);
		}

		// 衝突している状態だったら脱出した状態にする
		if (colliderB->GetCollisionState() == CollisionFlags::STAY) {
			colliderB->SetCollisionState(CollisionFlags::EXIT);
			colliderB->OnCollision(*colliderA);
		} else {
			colliderB->SetCollisionState(CollisionFlags::NONE);
		}
	}
}
