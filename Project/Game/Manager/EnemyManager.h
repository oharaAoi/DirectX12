#pragma once
// c++
#include <memory>
#include <list>
#include <vector>
#include <algorithm>
// game
#include "Game/GameObject/Enemy/NormalEnemy.h"

/// <summary>
/// Enemyを管理するクラス
/// </summary>
class EnemyManager {
public:

	EnemyManager();
	~EnemyManager();

	void Init();
	void Update();
	void Draw() const;

	/// <summary>
	/// 一番近い敵を探索する
	/// </summary>
	void CheckNearEnemyList();

public:

	std::shared_ptr<NormalEnemy> GetNearEnemy(uint32_t& index);

	std::list<std::shared_ptr<NormalEnemy>>& GetNormalEnemyList() { return normalEnemyList_; }

	void SetPlayerPos(const Vector3& playerPos) { playerPos_ = playerPos; }

	const uint32_t GetDownNum() const { return downNum_; }

private:

	void Pop(const Vector3& pos = Vector3::ZERO());

	void RandomPop();

#ifdef _DEBUG
	void Debug_Gui();

public:
	void Debug_Draw();
#endif

private:

	std::list<std::shared_ptr<NormalEnemy>> normalEnemyList_;

	std::vector<std::shared_ptr<NormalEnemy>> nearNormalEnemyList_;

	uint32_t downNum_;

	// ------------------- newEnemyParameter ------------------- //
	Vector3 newEnemyPos_;

	// ------------------- playerの情報 ------------------- //
	Vector3 playerPos_;

	// ------------------- 出現の情報 ------------------- //
	float popTime_ = 0.0f;
	float popTimeLimit_ = 8.0f;
	int popNum_ = 4;

	bool isStop_ = false;
};

