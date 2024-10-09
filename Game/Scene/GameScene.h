#pragma once
#include "Engine.h"
#include "Game/Scene/BaseScene.h"
#include "Engine/Lib/Transform.h"
#include "Engine/Utilities/DrawUtils.h"
#include "Game/Camera/BaseCamera.h"
#include "Game/Camera/DebugCamera.h"
#include "Game/WorldObject/Skydome.h"

class GameScene 
	: public BaseScene {
public:

	GameScene();
	~GameScene();

	void Finalize() override;
	void Init() override;
	void Load()  override;
	void Update() override;
	void Draw() const override;

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	// ------------------- Camera ------------------- //
	std::unique_ptr<BaseCamera> mainCamera_;

	// ------------------- WorldObject ------------------- //
	std::unique_ptr<Skydome> skydome_;

};
