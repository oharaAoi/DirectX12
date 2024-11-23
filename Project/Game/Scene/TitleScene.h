#pragma once
#include "Game/Scene/BaseScene.h"

class TitleScene
	: public BaseScene {
public:

	TitleScene();
	~TitleScene() override;

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

#ifdef _DEBUG
	void ImGuiDraw();
#endif

};

