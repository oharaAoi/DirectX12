#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include "Engine/GameObject/BaseGameObject.h"

class BossCorePullUI {
public:

	BossCorePullUI();
	~BossCorePullUI();

	void Init();
	void Update(float hp, float hpLimit);
	void Draw();

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:
	Vector3 pos_;
	std::unordered_map<std::string, std::unique_ptr<BaseGameObject>> objs3d_;
};

