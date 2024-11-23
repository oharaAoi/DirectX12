#pragma once
#include "Game/Scene/BaseScene.h"

class GameClearScene 
	: public BaseScene {
public:

	GameClearScene();
	~GameClearScene() override;

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

#ifdef _DEBUG
	void ImGuiDraw();
#endif

};
