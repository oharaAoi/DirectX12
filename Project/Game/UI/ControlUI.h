#pragma once
#include "Engine/Assets/2d/Sprite.h"

class ControlUI {
public:

	ControlUI();
	~ControlUI();

	void Init();
	void Update();
	void Draw() const;

private:

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	std::unique_ptr<Sprite> controlUI_[2];
	Vector2 controlUIPos_[2] = { {1000, 580.0f},{1000, 400.0f} };

};

