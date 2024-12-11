#pragma once
#include "Game/Interface/ICharactorState.h"

class Player;

class PlayerAttackState :
    public ICharactorState {
public:

	struct Work {
		float time;
		float timeLimit;
	};

public:
	PlayerAttackState(Player* pPlayer) : pPlayer_(pPlayer) { Init(); };
	~PlayerAttackState();

	void Init() override;
	void Update() override;

#ifdef _DEBUG
	void Debug_Gui() override;
#endif // _DEBUG

private:

	Player* pPlayer_ = nullptr;

	Work work_;
};

