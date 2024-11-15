#pragma once
#include <vector>
#include "Engine.h"
#include "Game/Scene/BaseScene.h"

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


<<<<<<< HEAD
	// --- eyePos/view/Projection ------------------- //
	Vector3 eyePos_;
	Matrix4x4 viewMat_;
	Matrix4x4 projectionMat_;

	// --- Camera ---------------------------------- //
	std::unique_ptr<DebugCamera> debugCamera_;
	bool isDebugCamera_;

	std::unique_ptr<FollowCamera> followCamera_;

	// --- WorldObject ----------------------------- //
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Field> field_;
	
	// --- GameObject ------------------------------ //
	std::unique_ptr<Player> player_;
	std::unique_ptr<Boss> boss_;
	
	std::unique_ptr<TestCollisionObj> testCollisionObj_;
	std::unique_ptr<TestCollisionObj> testCollisionObj2_;
	std::unique_ptr<TestCollisionObj> testCollisionObj3_;

	// --- Manager --------------------------------- //
	std::unique_ptr<EnemyManager> enemyManager_;
	std::unique_ptr<CollisionManager> collisionManager_;

	// --- UI -------------------------------------- //
	
	// --- Edier ----------------------------------- //
	std::unique_ptr<BossAttackEditer> bossLeftAttackEditer_;
	std::unique_ptr<BossAttackEditer> bossRightAttackEditer_;
	
=======
>>>>>>> main
};
