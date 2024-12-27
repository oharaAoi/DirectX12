#pragma once
#include "Game/Interface/ICharactorState.h"

class Player;

class PlayerDefaultState :
    public ICharactorState {
public:

	PlayerDefaultState(Player* pPlayer) : pPlayer_(pPlayer) { Init(); };
	~PlayerDefaultState();

	void Init() override;
	void Update() override;

#ifdef _DEBUG
	void Debug_Gui() override;
#endif // _DEBUG

private:

	Player* pPlayer_ = nullptr;
};

