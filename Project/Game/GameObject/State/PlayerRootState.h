#pragma once
#include "Game/GameObject/State/BaseObjectState.h"
#include "Engine/Math/MyRandom.h"

class Player;

/// <summary>
/// Playerの通常の状態
/// </summary>
class PlayerRootState :
	public BaseObjectState {
public:

	PlayerRootState(Player* pPlayer) : pPlayer_(pPlayer) { Init(); };
	~PlayerRootState();

	void Init() override;
	void Update() override;

#ifdef _DEBUG
	void Debug_Gui() override;
#endif // _DEBUG

private:

	Player* pPlayer_ = nullptr;

};

