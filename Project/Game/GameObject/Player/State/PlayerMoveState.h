#pragma once
#include "Game/Interface/ICharactorState.h"
#include "Engine/Math/Vector3.h"

class Player;

class PlayerMoveState :
    public ICharactorState {
public:

	PlayerMoveState(Player* pPlayer) : pPlayer_(pPlayer) { Init(); };
	~PlayerMoveState();

	void Init() override;
	void Update() override;

	void Jump();

	void Move();

#ifdef _DEBUG
	void Debug_Gui() override;
#endif // _DEBUG

private:

	Player* pPlayer_ = nullptr;

	Vector3 velocity_;
	Vector3 acceleration_;

	bool preIsJump_;
};

