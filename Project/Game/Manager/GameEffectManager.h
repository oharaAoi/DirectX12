#pragma once
#include <list>
#include <memory>
#include "Game/GameEffect/IGameEffect.h"
#include "Engine/Assets/WorldTransform.h"

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

	static void AddEffect(IGameEffect* gameEffect, const Vector3& pos = Vector3::ZERO(), WorldTransform* worldTransform = nullptr);
	static void AddNewEffect(std::unique_ptr<IGameEffect> gameEffect, const Vector3& pos = Vector3::ZERO(), WorldTransform* worldTransform = nullptr);

private:

	void AddEffectList(IGameEffect* gameEffect, const Vector3& pos, WorldTransform* worldTransform);
	void AddNewEffectList(std::unique_ptr<IGameEffect> gameEffect, const Vector3& pos, WorldTransform* worldTransform);

private:

	std::list<IGameEffect*> effectList_;
	std::list<std::unique_ptr<IGameEffect>> newEffectList_;

	std::unique_ptr<GpuParticle> gpuParticle_;
};

