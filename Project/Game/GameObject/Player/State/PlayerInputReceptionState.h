#pragma once
#include "Game/Interface/ICharactorState.h"

class Player;

/// <summary>
/// Playerの次の行動の入力を判定するクラス
/// </summary>
class PlayerInputReceptionState :
    public ICharactorState {
public:

    struct Work {
        float time;
    };

public:

    PlayerInputReceptionState(Player* pPlayer) : pPlayer_(pPlayer) { Init(); };
    ~PlayerInputReceptionState();

    void Init() override;
    void Update() override;

#ifdef _DEBUG
    void Debug_Gui() override;
#endif // _DEBUG


private:
    Player* pPlayer_ = nullptr;

    Work work_;
};

