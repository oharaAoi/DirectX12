#pragma once
#include "Game/GameObject/State/BaseObjectState.h"
#include "Engine/Math/MyRandom.h"

class BossCore;

class BossCoreTutorialState :
	public BaseObjectState {

public:

	BossCoreTutorialState(BossCore* pCore) : pCore_(pCore) { Init(); };
	~BossCoreTutorialState();

	void Init() override;
	void Update() override;

#ifdef _DEBUG
	void Debug_Gui() override;
#endif // _DEBUG

private:

	BossCore* pCore_ = nullptr;

	Vector3 pos_{};
};

