#pragma once
#include "Game/Interface/ICharactorState.h"

class NormalEnemy;

class NormalEnemyHitedState :
	public ICharactorState {
public:

	NormalEnemyHitedState(NormalEnemy* pEnemy) : pEnemy_(pEnemy) { Init(); };
	~NormalEnemyHitedState();

	void Init() override;
	void Update() override;

#ifdef _DEBUG
	void Debug_Gui() override {};
#endif // _DEBUG

private:

	NormalEnemy* pEnemy_ = nullptr;

};

