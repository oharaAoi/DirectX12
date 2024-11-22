#pragma once
#include "Game/GameObject/State/BaseObjectState.h"

class Boss;

class BossTransitionState : public BaseObjectState {
public:

	BossTransitionState(Boss* pBoss) : pBoss_(pBoss) { Init(); };
	~BossTransitionState();

	void Init()override;
	void Update() override;

#ifdef _DEBUG
	void Debug_Gui() override;
#endif // _DEBUG

private:

	Boss* pBoss_ = nullptr;

};

