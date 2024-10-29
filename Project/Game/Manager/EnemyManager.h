#pragma once
#include <memory>
#include <list>
#include "Game/GameObject/Enemy.h"
#include "Engine/Math/MyRandom.h"

class EnemyManager {
public:

	EnemyManager();
	~EnemyManager();

	void Init();
	void Update();
	void Draw() const;

	void AddList(const Vector3& popPos);

	void SetPlayerPos(const Vector3& pos) { playerPos_ = pos; }
	void SetPlayerForward(const Vector3& forward) { playerForward_ = forward; }

private:

	// enemyのリスト
	std::list<std::unique_ptr<Enemy>> enemyList_;

	// --- playerの情報 ----------------------------- //
	// playerの座標
	Vector3 playerPos_;
	// playerの前方向のベクトル
	Vector3 playerForward_;
	
	// --- Parameter ------------------------------- //
	float popTime_;
};

