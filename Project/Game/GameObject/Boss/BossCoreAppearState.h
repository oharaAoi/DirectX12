#pragma once
#include "Game/GameObject/State/BaseObjectState.h"
#include "Engine/Math/MyRandom.h"

class BossCore;

class BossCoreAppearState :
	public BaseObjectState {
public:

	BossCoreAppearState(BossCore* pCore) : pCore_(pCore) { Init(); };
	~BossCoreAppearState();

	void Init() override;
	void Update() override;

#ifdef _DEBUG
	void Debug_Gui() override;
#endif // _DEBUG

private:

	BossCore* pCore_ = nullptr;
	float moveTime_ = 0.0f;
	float appearTime_ = 10.0f;

	Vector3 defaultPosition_{};
	Vector3 middlePosition_{};
	Vector3 endPosition_{};

};

