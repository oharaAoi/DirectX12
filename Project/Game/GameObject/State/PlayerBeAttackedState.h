#pragma once
#include "Game/GameObject/State/BaseObjectState.h"
#include "Engine/Math/MyRandom.h"

class Player;

/// <summary>
/// Playerが攻撃を受けた際の状態
/// </summary>
class PlayerBeAttackedState :
	public BaseObjectState {

public:

	struct Work {
		float knockBackTime;
	};

public:

	PlayerBeAttackedState(Player* pPlayer) : pPlayer_(pPlayer) { Init(); };
	~PlayerBeAttackedState();

	void Init() override;
	void Update() override;

#ifdef _DEBUG
	void Debug_Gui() override;
#endif // _DEBUG

private:

	Player* pPlayer_ = nullptr;

	Work work_;

	Vector3 knockBackVelocity_;
	float knockBackSpeed_;
};

