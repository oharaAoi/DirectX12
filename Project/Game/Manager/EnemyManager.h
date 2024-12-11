#pragma once
#include <memory>
#include <list>
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

private:

	void Pop(const Vector3& pos = Vector3::ZERO());

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	std::list<std::unique_ptr<NormalEnemy>> normalEnemyList_;

};

