#pragma once
#include "Engine/GameObject/BaseGameObject.h"

class BossEye : public BaseGameObject {
public:

	BossEye();
	~BossEye();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void AdaptAdjustment();

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	std::string groupName_ = "boss_eye";

};

