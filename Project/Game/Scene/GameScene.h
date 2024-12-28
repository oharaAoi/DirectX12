#pragma once
// C++
#include <vector>
// Engine
#include "Engine.h"
#include "Engine/Manager/CollisionManager.h"
// Game
#include "Game/Scene/BaseScene.h"
#include "Game/Camera/DebugCamera.h"
#include "Game/Camera/FollowCamera.h"
#include "Game/WorldObject/Ground.h"
#include "Game/WorldObject/Skydome.h"
#include "Game/GameObject/Player/Player.h"
#include "Game/Manager/EnemyManager.h"

class GameScene 
	: public BaseScene {
public:

	GameScene();
	~GameScene();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

#ifdef _DEBUG
	void Debug_Gui() override;
#endif

private:

	// ------------------- camera ------------------- //
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;

	// ------------------- worldObject ------------------- //
	std::unique_ptr<Ground> ground_ = nullptr;
	std::unique_ptr<Skydome> skydome_ = nullptr;

	// ------------------- gameObject ------------------- //
	std::unique_ptr<Player> player_ = nullptr;

	// ------------------- manager ------------------- //
	std::unique_ptr<EnemyManager> enemyManager_ = nullptr;
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;

	// ------------------- parameter ------------------- //
	bool isDebug_ = false;

};
