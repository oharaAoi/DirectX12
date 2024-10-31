#pragma once
#include "Engine/GameObject/BaseGameObject.h"

class BossLeftHand : public BaseGameObject {
public:

	BossLeftHand();
	~BossLeftHand();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

#ifdef _DEBUG
	void Debug_Gui();
#endif


private:


};

