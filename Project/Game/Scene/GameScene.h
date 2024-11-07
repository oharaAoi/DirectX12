#pragma once
#include <vector>
#include "Engine.h"
#include "Game/Scene/BaseScene.h"
#include "Game/Camera/Camera.h"
#include "Game/Camera/RailCamera.h"
#include "Game/Camera/DebugCamera.h"
#include "Game/WorldObject/Skydome.h"
#include "Game/Editer/RailPointEditer.h"
#include "Game/WorldObject/Rail.h"
#include "Game/GameObject/Player.h"
#include "Game/GameObject/PlayerBullet.h"
#include "Game/Manager/EnemyManager.h"
#include "Engine/Manager/CollisionManager.h"
#include "Game/UI/Reticle.h"
#include "Game/UI/KnockDownEnemy.h"
#include "Game/UI/TotalScore.h"

class GameScene 
	: public BaseScene {
public:

	GameScene();
	~GameScene();

	void Init() override;
	void Update() override;
	void Draw() const override;

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	// --- eyePos/view/Projection ------------------- //
	Vector3 eyePos_;
	Matrix4x4 viewMat_;
	Matrix4x4 projectionMat_;

	// --- Camera ---------------------------------- //
	std::unique_ptr<RailCamera> mainCamera_;
	std::unique_ptr<DebugCamera> debugCamera_;
	bool isDebugCamera_;

	// --- WorldObject ----------------------------- //
	std::unique_ptr<Skydome> skydome_;
	std::vector<std::unique_ptr<Rail>> rails_;

	// --- GameObject ------------------------------ //
	std::unique_ptr<Player> player_;
	std::unique_ptr<PlayerBullet> playerBullets_[2];

	// --- Manager --------------------------------- //
	std::unique_ptr<EnemyManager> enemyManager_;
	std::unique_ptr<CollisionManager> collisionManager_;

	// --- UI -------------------------------------- //
	std::unique_ptr<Reticle> reticle_;
	std::unique_ptr<KnockDownEnemy> knockDownEnemy_;
	std::unique_ptr<TotalScore> totalScore_;

	// --- Edier ----------------------------------- //
	std::unique_ptr<RailPointEditer> railPointEditer_;
	
};
