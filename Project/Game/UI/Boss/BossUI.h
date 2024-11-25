#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include "Engine/2d/Sprite.h"
#include "Engine/GameObject/BaseGameObject.h"
#include "Game/GameObject/Boss/Boss.h"

class BossUI {
public:

	BossUI(Boss* pBoss);
	~BossUI();

	void Init();
	void Update(float bossHp, const Matrix4x4& vpvpMat);
	void Draw();

	void Draw3dObject(bool canAttackBoss);

	void AdaptAdjustment();

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	std::string groupName_ = "BossUI";

	Boss* pBoss_;

	std::unordered_map<std::string, std::unique_ptr<Sprite>> sprites_;

	std::unordered_map<std::string, std::unique_ptr<BaseGameObject>> planes_;

	float uiTime_ = 0.0f;
	float maxVal_ = 2.5f;
	float minVal_ = 2.0f;

};

