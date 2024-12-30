#pragma once
#include "Game/Interface/ICharactorState.h"

class NormalEnemy;

class NormalEnemyMoveState :
	public ICharactorState {
public:

	NormalEnemyMoveState(NormalEnemy* pEnemy) : pEnemy_(pEnemy) { Init(); };
	~NormalEnemyMoveState();

	void Init() override;
	void Update() override;

#ifdef _DEBUG
	void Debug_Gui() override;
#endif // _DEBUG

private:

	void Move();

private:

	NormalEnemy* pEnemy_ = nullptr;

	Vector3 velocity_;
	Vector3 distance_;
};

