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

	NormalEnemy* GetNearEnemy(uint32_t& index);

	std::list<std::unique_ptr<NormalEnemy>>& GetNormalEnemyList() { return normalEnemyList_; }

	void SetPlayerPos(const Vector3& playerPos) { playerPos_ = playerPos; }

private:

	void Pop(const Vector3& pos = Vector3::ZERO());

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	std::list<std::unique_ptr<NormalEnemy>> normalEnemyList_;

	std::vector<NormalEnemy*> nearNormalEnemyList_;

	// ------------------- newEnemyParameter ------------------- //
	Vector3 newEnemyPos_;

	// ------------------- playerの情報 ------------------- //
	Vector3 playerPos_;

};

