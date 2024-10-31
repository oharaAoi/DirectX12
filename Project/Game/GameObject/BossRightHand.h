#pragma once
#include "Engine/GameObject/BaseGameObject.h"

/// <summary>
/// Bossの右手
/// </summary>
class BossRightHand : public BaseGameObject {
public:

	BossRightHand();
	~BossRightHand();

	void Finalize() override;
	void Init() override;
	void Update() override;
	void Draw() const override;

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:


};

