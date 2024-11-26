#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include "Engine/GameObject/BaseGameObject.h"

class BaseBossHand;

class BossHandHpUI {
public:

	BossHandHpUI();
	~BossHandHpUI();

	void Init(BaseBossHand* hand);
	void Update(float hp, float hpLimit);
	void Draw();

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	std::unordered_map<std::string, std::unique_ptr<BaseGameObject>> objs3d_;
};
