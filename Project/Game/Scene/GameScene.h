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


};
