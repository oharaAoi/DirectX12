#pragma once
#include "Game/Interface/ICharactorState.h"

class Player;

enum class AttackStep {
	NONE,
	FIRST,
	SECOND,

	JUMPATTACK,
};

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

	void CombAttack();

	void JumpAttack();

#ifdef _DEBUG
	void Debug_Gui() override;
#endif // _DEBUG

private:

	Player* pPlayer_ = nullptr;

	Work work_;

	Vector3 velocity_;
	Vector3 acceleration_;
};

