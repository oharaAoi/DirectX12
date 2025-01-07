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

	void Move();

	void ConstrainToField(Vector3& translate);

#ifdef _DEBUG
	void Debug_Gui() override;
#endif // _DEBUG

private:

	NormalEnemy* pEnemy_ = nullptr;

	float time_;
	const float changeTime_ = 4.0f;
	Vector3 velocity_;
};

