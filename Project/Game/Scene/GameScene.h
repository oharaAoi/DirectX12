#pragma once
#include "Engine.h"
#include "Game/Scene/BaseScene.h"
#include "Game/Camera/Camera.h"
#include "Game/Camera/RailCamera.h"
#include "Game/Camera/DebugCamera.h"
#include "Game/WorldObject/Skydome.h"
#include "Game/Editer/RailPointEditer.h"
#include "Game/GameObject/Player.h"
#include "Game/GameObject/PlayerBullet.h"

class GameScene 
	: public BaseScene {
public:

	GameScene();
	~GameScene();

	void Init() override;
	void Update() override;
	void Draw() const override;

	void AddPlayerBulletList(const Vector3& pos, const Vector3& velocity);

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	// ------------------- eyePos/view/Projection ------------------- //
	Vector3 eyePos_;
	Matrix4x4 viewMat_;
	Matrix4x4 projectionMat_;

	// ------------------- Camera ------------------- //
	std::unique_ptr<RailCamera> mainCamera_;
	std::unique_ptr<DebugCamera> debugCamera_;
	bool isDebugCamera_;

	// ------------------- WorldObject ------------------- //
	std::unique_ptr<Skydome> skydome_;

	// ------------------- GameObject ------------------- //
	std::unique_ptr<Player> player_;
	std::list<std::unique_ptr<PlayerBullet>> playerBullets_;

	// ------------------- Edier ------------------- //
	std::unique_ptr<RailPointEditer> railPointEditer_;
	
};
