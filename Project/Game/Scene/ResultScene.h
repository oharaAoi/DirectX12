#pragma once
#include <memory>
#include <vector>
#include "Engine.h"
#include "Game/Scene/BaseScene.h"
#include "Engine/2d/Sprite.h"
#include "Engine/Audio/AudioPlayer.h"

class ResultScene : public BaseScene {
public:

	ResultScene();
	~ResultScene();

	void Init() override;
	void Update() override;
	void Draw() const override;

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	std::unique_ptr<Sprite> clear_;

	std::unique_ptr<Sprite> panel_;

};

