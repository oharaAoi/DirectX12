#pragma once
#include <vector>
#include <list>
#include "Engine.h"
#include "Game/Scene/BaseScene.h"
#include "Game/Camera/Camera.h"
#include "Game/Camera/FollowCamera.h"
#include "Game/Camera/DebugCamera.h"
#include "Game/WorldObject/Skydome.h"
#include "Game/WorldObject/Field.h"
#include "Game/GameObject/Player.h"
#include "Game/GameObject/Boss.h"
#include "Game/GameObject/Missile.h"
#include "Game/Manager/EnemyManager.h"
#include "Engine/Manager/CollisionManager.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Game/Editer/BossAttackEditer.h"
#include "TestCollisionObj.h"
#include "Game/GameObject/Fall.h"

class GameScene 
	: public BaseScene {
public:

	GameScene();
	~GameScene();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void AddMissile(const Vector3& targePos, const Vector3& firePos);

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	// --- 調整項目 ----------------------------------- //
	AdjustmentItem* adjustmentItem_;

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

	std::list<std::unique_ptr<Missile>> missileList_;
	
	std::unique_ptr<TestCollisionObj> testCollisionObj_;
	std::unique_ptr<TestCollisionObj> testCollisionObj3_;

	std::unique_ptr<Fall> fall_;
	
	// --- Manager --------------------------------- //
	std::unique_ptr<EnemyManager> enemyManager_;
	std::unique_ptr<CollisionManager> collisionManager_;

	// --- UI -------------------------------------- //
	
	// --- Edier ----------------------------------- //
	std::unique_ptr<BossAttackEditer> bossLeftAttackEditer_;
	std::unique_ptr<BossAttackEditer> bossRightAttackEditer_;
	
};
