#pragma once
#include "Game/Interface/ICharactorState.h"
#include "Engine/Math/Vector3.h"

class Player;

/// <summary>
/// 回避状態
/// </summary>
class PlayerAvoidanceState :
    public ICharactorState {
public:

public:

	PlayerAvoidanceState(Player* pPlayer) : pPlayer_(pPlayer) { Init(); };
	~PlayerAvoidanceState();

	void Init() override;
	void Update() override;

#ifdef _DEBUG
	void Debug_Gui() override;
#endif // _DEBUG

private:

	Player* pPlayer_ = nullptr;

};

