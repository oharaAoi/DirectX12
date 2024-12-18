#pragma once
#include <vector>
#include <list>
#include "Engine.h"
#include "Game/Scene/BaseScene.h"
#include "Game/Camera/Camera.h"
#include "Game/Camera/FollowCamera.h"
#include "Game/Camera/DebugCamera.h"
#include "Game/WorldObject/Skydome.h"
#include "Game/WorldObject/BackGround.h"
#include "Game/WorldObject/Field.h"
#include "Game/GameObject/Player/Player.h"
#include "Game/GameObject/Boss/Boss.h"
#include "Game/GameObject/Missile.h"
#include "Engine/Manager/CollisionManager.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Game/Editer/BossAttackEditer.h"
#include "TestCollisionObj.h"
#include "Game/GameObject/Fall.h"
#include "Game/GameObject/FallStone.h"
#include "Game/UI/Player/PlayerUI.h"
#include "Game/UI/Boss/BossUI.h"
#include "Game/UI/Boss/BossHpUI.h"
#include "Game/UI/Boss/BossCorePullUI.h"
#include "Game/UI/Panel.h"
#include "Game/Manager/AnimetionEffectManager.h"
#include "Game/Manager/GameObjectManager.h"
#include "Engine/Audio/AudioPlayer.h"
#include "Game/UI/ClutchGauge.h"
#include "Game/Manager/GameEffectManager.h"

class GameScene 
	: public BaseScene {
public:

	GameScene();
	~GameScene();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

private:

	void UpdateWorldObject();

	void UpdateGameObject();

	void UpdateUI();

	void UpdateManager();

	/// <summary>
	/// ボスの状態を変化させる
	/// </summary>
	void BossFormTransition();

	/// <summary>
	/// ゲーム開始時のの更新処理
	/// </summary>
	void AppearUpdate();

public:

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
	std::unique_ptr<BackGround> backGround_;
	std::unique_ptr<Field> field_;
	
	// --- GameObject ------------------------------ //
	std::unique_ptr<Player> player_;
	std::unique_ptr<Boss> boss_;

	std::list<std::unique_ptr<Missile>> missileList_;

	std::unique_ptr<TestCollisionObj> rightSnaggeObj_;
	std::unique_ptr<TestCollisionObj> leftSnaggeObj_;

	
	std::unique_ptr<Fall> fall_;
	std::unique_ptr<FallStone> fallStone_;

	// --- Manager --------------------------------- //
	std::unique_ptr<CollisionManager> collisionManager_;

	GameObjectManager* gameObjectManager_;

	AnimetionEffectManager* animationEffectManager_;

	GameEffectManager* gameEffectManager_;

	// --- UI -------------------------------------- //
	std::unique_ptr<PlayerUI> playerUI_;
	std::unique_ptr<BossUI> bossUI_;

	std::unique_ptr<Panel> panel_;

	std::unique_ptr<BossHpUI> bossHpUI_;

	std::unique_ptr<BossCorePullUI> bossCorePullUI_;

	std::unique_ptr<ClutchGauge> clutchGauge_;
	
	// --- Edier ----------------------------------- //
	std::unique_ptr<BossAttackEditer> bossLeftAttackEditer_;
	std::unique_ptr<BossAttackEditer> bossRightAttackEditer_;

	// --- audio -------------------------------------- //
	std::unique_ptr<AudioPlayer> bgm_;
	

	//=============================
	// ↓　パラメータ
	//=============================

	float bossFormTransitionTime_ = 0.0f;
	float bossFormTransitionTimeLimit_ = 1.5f;

	bool finishAppear_ = false;

	bool isPlayerAutoMove_ = true;

	bool isNextScene_ = false; 

#ifdef _DEBUG
	Vector3 bombPopPos_;
#endif

};
