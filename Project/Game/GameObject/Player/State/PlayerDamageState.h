#pragma once
#include "Game/Interface/ICharactorState.h"

class Player;

class PlayerDamageState :
    public ICharactorState {
public:	// メンバ構造体

	struct Work {
		float time;
		float timeLimit;
		float strength;
	};

public:

	PlayerDamageState(Player* pPlayer) : pPlayer_(pPlayer) { Init(); };
	~PlayerDamageState();

	void Init() override;
	void Update() override;

	void KnockBack();

#ifdef _DEBUG
	void Debug_Gui() override;
#endif // _DEBUG

private:

	Player* pPlayer_ = nullptr;

	Work work_;
};

