#pragma once
#include "Game/Interface/ICharactorState.h"

class NormalEnemy;

class NormalEnemyAttackState :
	public ICharactorState {
public:

	NormalEnemyAttackState(NormalEnemy* pEnemy) : pEnemy_(pEnemy) { Init(); };
	~NormalEnemyAttackState();

	void Init() override;
	void Update() override;

#ifdef _DEBUG
	void Debug_Gui() override;
#endif // _DEBUG

private:

	NormalEnemy* pEnemy_ = nullptr;
};

