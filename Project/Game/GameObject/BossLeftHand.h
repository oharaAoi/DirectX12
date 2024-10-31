#pragma once
#include <string>
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/GameObject/BaseGameObject.h"

class BossLeftHand : public BaseGameObject {
public:

	BossLeftHand();
	~BossLeftHand();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

	void AdaptAdjustment();

#ifdef _DEBUG
	void Debug_Gui();
#endif


private:

	std::string groupName_ = "BossLeftHand";

};

