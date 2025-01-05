#pragma once
#include "Engine/Assets/2d/Sprite.h"

class TitlteUI {
public:

	TitlteUI();
	~TitlteUI();

	void Init();
	void Update();
	void Draw() const;

private:

	std::unique_ptr<Sprite> titleSprite_;
	std::unique_ptr<Sprite> pushA_;

};

