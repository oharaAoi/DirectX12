#pragma once
#include "Game/GameObject/Boss/Boss.h"

class BossController {
public:

	BossController();
	~BossController();

	void Init(Boss* boss);
	void Update();

private:

	Boss* boss_;

};

