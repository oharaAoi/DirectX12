#pragma once
#include <memory>
#include <list>
#include "Game/GameObject/Bomb.h"
#include "Game/GameObject/Missile.h"

class GameObjectManager {
public:

	GameObjectManager() = default;
	~GameObjectManager();
	GameObjectManager(const GameObjectManager&) = delete;
	const GameObjectManager& operator=(const GameObjectManager&) = delete;

	static GameObjectManager* GetInstance();

	void Finalize();

	void Init();

	void Update();

	void Draw() const;

	void DrawUI() const;

	static void PopBomb(const Vector3& popPos);

private:

	void AddBomb(const Vector3& popPos);

public:

#ifdef _DEBUG
	void Debug_Draw();
#endif

	std::list<std::unique_ptr<Bomb>>& GetBombList() { return bombList_; }

private:

	std::list<std::unique_ptr<Bomb>> bombList_;

};

