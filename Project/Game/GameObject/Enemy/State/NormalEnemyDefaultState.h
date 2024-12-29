#pragma once
#include "Game/Interface/ICharactorState.h"

class NormalEnemy;

class NormalEnemyDefaultState :
	public ICharactorState {
public:

	NormalEnemyDefaultState(NormalEnemy* pEnemy) : pEnemy_(pEnemy) { Init(); };
	~NormalEnemyDefaultState();

	void Init() override;
	void Update() override;

#ifdef _DEBUG
	void Debug_Gui() override;
#endif // _DEBUG

private:

	NormalEnemy* pEnemy_ = nullptr;
};

