#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include "Engine/2d/Sprite.h"
#include "Engine/GameObject/BaseGameObject.h"
#include "Game/GameObject/Boss/Boss.h"

class BossHpUI {
public:

	BossHpUI(Boss* pBoss);
	~BossHpUI();

	void Init();
	void Update(float bossHp);
	void Draw();

	void ScaleUpBossHp();

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	Boss* pBoss_;

	std::unordered_map<std::string, std::unique_ptr<BaseGameObject>> obj3d_;

	bool isScaleUpBossHp_ = false;
	float scaleUpTime_ = 0.0f;

	float bossHp_;
};

