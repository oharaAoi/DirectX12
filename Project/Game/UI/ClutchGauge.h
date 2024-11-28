#pragma once
#include "Engine/2d/Sprite.h"

class ClutchGauge {
public:

	ClutchGauge();
	~ClutchGauge();

	void Init();

	void Update(float coreEne, float coreMax, float fallEne, float fallMax, bool isCore, bool isFall);

	void Draw() const;

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	std::unique_ptr<Sprite> core_[2];
	std::unique_ptr<Sprite> fall_[2];

	Vector2 pos_ = {1180.0f, 400.0f};

	bool isCore_;
	bool isFall_;
};

