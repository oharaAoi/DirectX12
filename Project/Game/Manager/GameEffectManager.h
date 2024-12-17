#pragma once
#include <list>
#include <memory>
#include "Game/GameEffect/IGameEffect.h"

/// <summary>
/// Effectを管理するクラス
/// </summary>
class GameEffectManager {
public:

	GameEffectManager() = default;
	~GameEffectManager();

	GameEffectManager(const GameEffectManager&) = delete;
	const GameEffectManager& operator=(const GameEffectManager&) = delete;

	static GameEffectManager* GetInstance();

	void Finalize();
	void Init();
	void Update();
	void Draw();

	static void AddEffect(IGameEffect* gameEffect);

private:

	void AddEffectList(IGameEffect* gameEffect);

private:

	std::list<std::unique_ptr<IGameEffect>> effectList_;

};

