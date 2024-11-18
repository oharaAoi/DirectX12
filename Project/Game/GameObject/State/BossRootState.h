#pragma once
#include "BaseObjectState.h"

class Boss;

class BossRootState :
    public BaseObjectState {
public: // メンバ構想体

    /// <summary>
    /// RootState状態の時に行う処理をまとめた構造体
    /// </summary>
    struct Work {
        float waitTime;
    };

public:

    BossRootState(Boss* pBoss) : pBoss_(pBoss) { Init(); };
    ~BossRootState();

    void Init() override;
    void Update() override;

private:

    Boss* pBoss_ = nullptr;

    Work work_;

};

