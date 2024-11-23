#pragma once
#include "Game/Scene/BaseScene.h"

class GameOverScene
	: public BaseScene {
public:

	GameOverScene();
	~GameOverScene() override;

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

#ifdef _DEBUG
	void ImGuiDraw();
#endif

};
