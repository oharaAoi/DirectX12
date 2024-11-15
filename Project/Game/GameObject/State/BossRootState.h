#pragma once
#include "BaseObjectState.h"

class Boss;

class BossRootState :
    public BaseObjectState {
public:

    BossRootState(Boss* pBoss) : pBoss_(pBoss) { Init(); };
    ~BossRootState();

    void Init() override;
    void Update() override;

private:

    Boss* pBoss_ = nullptr;

};

