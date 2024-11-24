#pragma once
#include "Game/GameObject/State/BaseObjectState.h"
#include "Engine/Math/MyRandom.h"

class BossCore;

class BossCoreDefaultState :
	public BaseObjectState {
public:

	BossCoreDefaultState(BossCore* pCore) : pCore_(pCore) { Init(); };
	~BossCoreDefaultState();

	void Init() override;
	void Update() override;

#ifdef _DEBUG
	void Debug_Gui() override;
#endif // _DEBUG

private:

	BossCore* pCore_ = nullptr;

};

