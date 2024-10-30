#pragma once
#include <vector>
#include "Engine.h"
#include "Game/Scene/BaseScene.h"
#include "Game/Camera/Camera.h"
#include "Game/Camera/DebugCamera.h"
#include "Game/WorldObject/Skydome.h"
#include "Game/GameObject/Player.h"
#include "Game/Manager/EnemyManager.h"
#include "Engine/Manager/CollisionManager.h"

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
	std::unique_ptr<DebugCamera> debugCamera_;
	bool isDebugCamera_;

	// --- WorldObject ----------------------------- //
	std::unique_ptr<Skydome> skydome_;
	
	// --- GameObject ------------------------------ //
	std::unique_ptr<Player> player_;
	
	// --- Manager --------------------------------- //
	std::unique_ptr<EnemyManager> enemyManager_;
	std::unique_ptr<CollisionManager> collisionManager_;

	// --- UI -------------------------------------- //
	
	// --- Edier ----------------------------------- //
	
};
