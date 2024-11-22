#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include "Engine/2d/Sprite.h"

class BossUI {
public:

	BossUI();
	~BossUI();

	void Init();
	void Update();
	void Draw();

	void AdaptAdjustment();

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	std::string groupName_ = "BossUI";

	std::unordered_map<std::string, std::unique_ptr<Sprite>> sprites_;


};

