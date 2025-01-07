#pragma once
// C++
#include <vector>
// Engine
#include "Engine.h"
#include "Engine/System/Manager/CollisionManager.h"
#include "Engine/System/Audio/AudioPlayer.h"
// Game
#include "Game/Scene/BaseScene.h"
#include "Game/Camera/DebugCamera.h"
#include "Game/Camera/FollowCamera.h"
#include "Game/WorldObject/Ground.h"
#include "Game/WorldObject/Skydome.h"
#include "Game/GameObject/Player/Player.h"
#include "Game/Manager/EnemyManager.h"
#include "Game/System/LockOn/LockOn.h"
#include "Game/Effect/Trail.h"
#include "Game/UI/Score.h"
#include "Game/UI/FadePanel.h"
#include "Game/UI/ControlUI.h"

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

	const uint32_t kMaxTarget_ = 30;

	// ------------------- camera ------------------- //
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;

	// ------------------- worldObject ------------------- //
	std::unique_ptr<Ground> ground_ = nullptr;
	std::unique_ptr<Skydome> skydome_ = nullptr;

	// ------------------- gameObject ------------------- //
	std::unique_ptr<Player> player_ = nullptr;

	// ------------------- system ------------------- //
	std::unique_ptr<LockOn> lockOn_ = nullptr;

	// ------------------- UI ------------------- //
	std::unique_ptr<Score> score_;
	std::unique_ptr<ControlUI> controlUI_;

	std::unique_ptr<FadePanel> panel_;

	// ------------------- Effect ------------------- //
	std::unique_ptr<Trail> trail_;

	// ------------------- manager ------------------- //
	std::unique_ptr<EnemyManager> enemyManager_ = nullptr;
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;


	// ------------------- Audio ------------------- //

	std::unique_ptr<AudioPlayer> bgm_;

	// ------------------- parameter ------------------- //
	bool isDebug_ = false;

	bool isClear_;

	bool isStop_ = false;

};
