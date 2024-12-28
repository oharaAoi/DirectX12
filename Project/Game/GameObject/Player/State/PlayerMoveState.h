#pragma once
#include "Game/Interface/ICharactorState.h"
#include "Engine/Math/Vector3.h"

class Player;

const float kThreshold = 0.7f;

class PlayerMoveState :
    public ICharactorState {
public:	// メンバ構造体

	struct Work {
		float speed;
	};

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

	bool isMoving_;

	Work work_;
};

